#include "list.h"
#include <iostream>

void InitBlock(EL_Stroka*& block, BlockType type) {
    block = new EL_Stroka;
    block->type = type;
    block->next = nullptr;
    block->next_word_block = nullptr;
    block->sentence_id = 0;

    switch (type) {
    case LETTERS:
        block->content.letters.data[0] = '\0';
        block->content.letters.size = 0;
        block->content.letters.is_word_part = false;
        break;
    case SPACES:
        block->content.spaces.count = 1;
        break;
    case PUNCTUATION:
        block->content.punctuation.symbol = '\0';
        break;
    case COMMA:
        block->content.comma.comma = ',';
        break;
    }
}

void InitLine(Form_Stroka& line) {
    line.head = line.tail = nullptr;
    line.count = 0;
}

void InitLevel(EL_V*& level) {
    level = new EL_V;
    InitLine(level->line);
    level->next = nullptr;
}

void InitDocument(Form_V& doc) {
    doc.head = doc.tail = nullptr;
    doc.total_sentences = 0;
}

void AddBlockToLine(Form_Stroka& line, EL_Stroka* block) {
    if (!line.head) {
        line.head = line.tail = block;
    }
    else {
        line.tail->next = block;
        line.tail = block;
    }
    line.count++;
}

void AddLevelToDoc(Form_V& doc, EL_V* level) {
    if (!doc.head) {
        doc.head = doc.tail = level;
    }
    else {
        doc.tail->next = level;
        doc.tail = level;
    }
}