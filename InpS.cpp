#include "InpS.h"
#include "list.h"
#include <fstream>
#include <iostream>

bool IsLetter(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

BlockType DetermineType(char c) {  // Новая функция определения типа
    if (IsLetter(c)) return LETTERS;
    if (c == ' ') return SPACES;
    if (c == ',') return COMMA;     // Отдельная обработка запятых
    return PUNCTUATION;             // Остальные знаки пунктуации
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
    int current_sentence = 0;
    bool in_sentence = false;

    char c;
    while (fin.get(c)) {
        if (c == '\n') {
            if (current_block) {
                AddBlockToLine(current_level->line, current_block);
                current_block = nullptr;
            }
            EL_V* new_level = new EL_V;
            InitLevel(new_level);
            AddLevelToDoc(doc, new_level);
            current_level = new_level;
            continue;
        }

        BlockType type = DetermineType(c); 

        if (current_block && current_block->type != type) {
            AddBlockToLine(current_level->line, current_block);
            current_block = nullptr;
        }

        if (!current_block) {
            InitBlock(current_block, type);
            current_block->sentence_id = in_sentence ? current_sentence : 0;
        }
        switch (type) {
        case LETTERS:
            if (current_block->content.letters.size >= current_block->content.letters.buffer_size) {
                int new_size = current_block->content.letters.buffer_size * 2;
                char* new_data = new char[new_size];
                for (int i = 0; i < current_block->content.letters.size; ++i) {
                    new_data[i] = current_block->content.letters.data[i];
                }
                delete[] current_block->content.letters.data;
                current_block->content.letters.data = new_data;
                current_block->content.letters.buffer_size = new_size;
            }
            current_block->content.letters.data[current_block->content.letters.size++] = c;
            if (!in_sentence) {
                current_sentence++;
                doc.total_sentences++;
                in_sentence = true;
            }
            current_block->sentence_id = current_sentence;
            break;

        case SPACES:
            current_block->content.spaces.count++;
            break;

        case COMMA: 
            AddBlockToLine(current_level->line, current_block);
            current_block = nullptr;
            break;

        case PUNCTUATION: 
            current_block->content.punctuation.symbol = c;
            in_sentence = false;
            AddBlockToLine(current_level->line, current_block);
            current_block = nullptr;
            break;
        }
    }

    if (current_block) AddBlockToLine(current_level->line, current_block);
    fin.close();
}

void PrintDocument(const Form_V& doc, std::ostream& out) {
    out << "\n===== Document =====\n";
    int level_num = 1;
    for (EL_V* level = doc.head; level; level = level->next) {
        out << "Level " << level_num++ << ":\n";
        for (EL_Stroka* block = level->line.head; block; block = block->next) {
            out << "  ";
            switch (block->type) {
            case LETTERS:
                out << "[LETTERS: \"";
                for (int i = 0; i < block->content.letters.size; ++i) {
                    out << block->content.letters.data[i];
                }
                out << "\" (Sent: " << block->sentence_id << ")]";
                break;
            case SPACES:
                out << "[SPACES: '" << block->content.spaces.spaceChar
                    << "' x" << block->content.spaces.count - 1
                    << " (Sent: " << block->sentence_id << ")]";
                break;
            case COMMA:  
                out << "[COMMA: '" << block->content.comma.comma
                    << "' (Sent: " << block->sentence_id << ")]";
                break;
            case PUNCTUATION:
                out << "[PUNCT: '" << block->content.punctuation.symbol
                    << "' (Sent: " << block->sentence_id << ")]";
                break;
            }
            out << "\n";
        }
        out << "----------------\n";
    }
    out << "Total sentences: " << doc.total_sentences << "\n";
    out << "====================\n\n";
}