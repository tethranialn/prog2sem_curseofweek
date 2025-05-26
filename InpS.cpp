#include "InpS.h"
#include "list.h"
#include <fstream>
#include <iostream>

bool IsLetter(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

BlockType DetermineType(char c) {
    if (IsLetter(c)) return LETTERS;
    if (c == ' ') return SPACES;
    if (c == ',') return COMMA;
    if (c == ';') return COMMA;
    if (c == ':') return COMMA;
    return PUNCTUATION;
}

void ReadDocument(Form_V& doc, const char* filename) {
    std::ifstream fin(filename);
    if (!fin) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    InitDocument(doc);
    EL_V* current_level = new EL_V;
    InitLevel(current_level);
    AddLevelToDoc(doc, current_level);

    EL_Stroka* current_block = nullptr;
    EL_Stroka* current_word = nullptr;
    int current_sentence = 1;
    bool in_sentence = false;
    char c;

    while (fin.get(c)) {
        if (c == '\r') continue; 
        if (c == '\n') {
            if (current_level->line.head) {
                EL_V* new_level = new EL_V;
                InitLevel(new_level);
                AddLevelToDoc(doc, new_level);
                current_level = new_level;
            }
            current_block = nullptr;
            current_word = nullptr;
            continue;
        }

        BlockType type = DetermineType(c);

        if (type != LETTERS) {
            current_word = nullptr;
        }

        if (current_block && current_block->type != type) {
            current_block = nullptr;
        }

        if (!current_block) {
            InitBlock(current_block, type);
            current_block->sentence_id = current_sentence;
            AddBlockToLine(current_level->line, current_block);
        }

        switch (type) {
        case LETTERS: {
            if (!current_word || current_word->content.letters.size >= 5) {
                if (current_word) {
                    InitBlock(current_block, LETTERS);
                    current_block->content.letters.is_word_part = true;
                    current_block->sentence_id = current_sentence;
                    current_word->next_word_block = current_block;
                    AddBlockToLine(current_level->line, current_block);
                }
                current_word = current_block;
            }

            if (current_word->content.letters.size < 5) {
                current_word->content.letters.data[current_word->content.letters.size++] = c;
                current_word->content.letters.data[current_word->content.letters.size] = '\0';
            }

            if (!in_sentence) {
                doc.total_sentences++;
                in_sentence = true;
            }
            break;
        }
        case SPACES:
            if (current_block->type == SPACES) {
                current_block->content.spaces.count++; 
            }
            break;
        case COMMA:
            current_block->content.comma.comma = ',';
            break;
        case PUNCTUATION:
            current_block->content.punctuation.symbol = c;
            if (in_sentence) {
                current_sentence++;
                in_sentence = false;
            }
            break;
        }
    }

    fin.close();
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