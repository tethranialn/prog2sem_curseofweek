#include "Del.h"
#include "InpS.h"
#include "list.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

void ReadDocument(Form_V& doc, const char* filename) {
    InitDocument(doc);
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cerr << "Error: Could not open input file '" << filename << "'. Please ensure the file exists in the same directory as the executable.\n";
        std::exit(1);
    }
    std::cout << "Successfully opened input file: " << filename << "\n";

    EL_V* currentLevel = new EL_V;
    if (!currentLevel) {
        std::cerr << "Error: Failed to allocate memory for document level.\n";
        in.close();
        std::exit(1);
    }
    InitLevel(currentLevel);
    AddLevelToDoc(doc, currentLevel);
    int sentenceId = 0;
    char c;
    bool inWord = false;
    EL_Stroka* currentWordBlock = nullptr;

    while (in.get(c)) {
        if (c == '\n') {
            EL_V* newLevel = new EL_V;
            if (!newLevel) {
                std::cerr << "Error: Failed to allocate memory for new document level.\n";
                in.close();
                DeleteDocument(doc);
                std::exit(1);
            }
            InitLevel(newLevel);
            AddLevelToDoc(doc, newLevel);
            currentLevel = newLevel;
            continue;
        }

        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            if (!inWord) {
                EL_Stroka* block = nullptr;
                InitBlock(block, LETTERS);
                if (!block) {
                    std::cerr << "Error: Failed to allocate memory for letter block.\n";
                    in.close();
                    DeleteDocument(doc);
                    std::exit(1);
                }
                block->sentence_id = sentenceId;
                AddBlockToLine(currentLevel->line, block);
                currentWordBlock = block;
                inWord = true;
            }

            int pos = currentWordBlock->content.letters.size;
            if (pos < 5) {
                currentWordBlock->content.letters.data[pos] = c;
                currentWordBlock->content.letters.size++;
                currentWordBlock->content.letters.data[pos + 1] = '\0';
            }
            else {
                EL_Stroka* partBlock = nullptr;
                InitBlock(partBlock, LETTERS);
                if (!partBlock) {
                    std::cerr << "Error: Failed to allocate memory for letter part block.\n";
                    in.close();
                    DeleteDocument(doc);
                    std::exit(1);
                }
                partBlock->content.letters.is_word_part = true;
                partBlock->sentence_id = sentenceId;
                partBlock->content.letters.data[0] = c;
                partBlock->content.letters.size = 1;
                partBlock->content.letters.data[1] = '\0';
                currentWordBlock->next_word_block = partBlock;
                currentWordBlock = partBlock;
            }
        }
        else {
            inWord = false;
            currentWordBlock = nullptr;

            if (c == ' ') {
                EL_Stroka* block = nullptr;
                InitBlock(block, SPACES);
                if (!block) {
                    std::cerr << "Error: Failed to allocate memory for space block.\n";
                    in.close();
                    DeleteDocument(doc);
                    std::exit(1);
                }
                block->sentence_id = sentenceId;
                block->content.spaces.count = 1;
                AddBlockToLine(currentLevel->line, block);
                int spaceCount = 1;
                while (in.peek() == ' ') {
                    in.get(c);
                    spaceCount++;
                }
                block->content.spaces.count = spaceCount;
            }
            else if (c == ',' || c == ';' || c == ':') {
                EL_Stroka* block = nullptr;
                InitBlock(block, COMMA);
                if (!block) {
                    std::cerr << "Error: Failed to allocate memory for comma block.\n";
                    in.close();
                    DeleteDocument(doc);
                    std::exit(1);
                }
                block->sentence_id = sentenceId;
                block->content.comma.comma = c;
                AddBlockToLine(currentLevel->line, block);
            }
            else if (c == '.' || c == '!' || c == '?') {
                EL_Stroka* block = nullptr;
                InitBlock(block, PUNCTUATION);
                if (!block) {
                    std::cerr << "Error: Failed to allocate memory for punctuation block.\n";
                    in.close();
                    DeleteDocument(doc);
                    std::exit(1);
                }
                block->sentence_id = sentenceId;
                block->content.punctuation.symbol = c;
                AddBlockToLine(currentLevel->line, block);
                sentenceId++;
                doc.total_sentences = sentenceId;
            }
        }
    }
    in.close();
}

void PrintDocument(const Form_V& doc, std::ostream& out) {
    out << "\n===== Document =====\n";
    int level_num = 1;
    for (EL_V* level = doc.head; level; level = level->next) {
        out << "Level " << level_num++ << ":\n";
        EL_Stroka* block = level->line.head;
        while (block) {
            if (block->type == LETTERS && block->content.letters.is_word_part) {
                block = block->next;
                continue;
            }
            out << "  ";
            switch (block->type) {
            case LETTERS: {
                out << "[WORD: \"" << block->content.letters.data << "\"";
                EL_Stroka* word_part = block->next_word_block;
                while (word_part) {
                    out << " -> \"" << word_part->content.letters.data << "\"";
                    word_part = word_part->next_word_block;
                }
                out << " (Sent: " << block->sentence_id << ")]";
                break;
            }
            case SPACES:
                out << "[SPACE x" << block->content.spaces.count << "]";
                break;
            case COMMA:
                out << "[COMMA: '" << block->content.comma.comma << "']";
                break;
            case PUNCTUATION:
                out << "[PUNCT: '" << block->content.punctuation.symbol << "']";
                break;
            }
            out << "\n";
            block = block->next;
        }
        out << "----------------\n";
    }
    out << "Total sentences: " << doc.total_sentences << "\n";
    out << "====================\n\n";
}

void PrintDocumentConcise(const Form_V& doc, std::ostream& out) {
    if (!doc.head) {
        out << "Document is empty.\n";
        return;
    }
    EL_V* level = doc.head;
    while (level) {
        if (level->line.head) {
            EL_Stroka* block = level->line.head;
            while (block) {
                if (block->type == LETTERS) {
                    out << block->content.letters.data;
                    EL_Stroka* part = block->next_word_block;
                    while (part) {
                        out << part->content.letters.data;
                        part = part->next_word_block;
                    }
                }
                else if (block->type == SPACES) {
                    for (int i = 0; i < block->content.spaces.count; i++) {
                        out << ' ';
                    }
                }
                else if (block->type == COMMA) {
                    out << block->content.comma.comma;
                }
                else if (block->type == PUNCTUATION) {
                    out << block->content.punctuation.symbol;
                }
                block = block->next;
            }
            out << "\n";
        }
        level = level->next;
    }
}