#pragma once
#ifndef LIST_H
#define LIST_H
#include <fstream>

enum BlockType { LETTERS, SPACES, PUNCTUATION, COMMA }; 

struct EL_Stroka {
    BlockType type;
    union {
        struct {
            char* data;
            int size;
            int buffer_size;
            bool is_word_part;
        } letters;
        struct {
            char spaceChar;
            int count;
        } spaces;
        struct {
            char symbol;
        } punctuation;
        struct {
            char comma; 
        } comma;
    } content;
    EL_Stroka* next_word_block;
    EL_Stroka* next;
    int sentence_id;
};

struct Form_Stroka {
    EL_Stroka* head;
    EL_Stroka* tail;
    int count;
};

struct EL_V {
    Form_Stroka line;
    EL_V* next;
};

struct Form_V {
    EL_V* head;
    EL_V* tail;
    int total_sentences;
};

void InitBlock(EL_Stroka*& block, BlockType type);
void InitLine(Form_Stroka& line);
void InitLevel(EL_V*& level);
void InitDocument(Form_V& doc);
void AddBlockToLine(Form_Stroka& line, EL_Stroka* block);
void AddLevelToDoc(Form_V& doc, EL_V* level);

#endif