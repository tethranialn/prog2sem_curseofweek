#include "list.h"

void InitDocument(Form_V& doc) {
    doc.head = nullptr;
    doc.total_sentences = 0;
}

void InitLevel(EL_V* level) {
    if (!level) {
        return;
    }
    level->line.head = nullptr;
    level->next = nullptr;
}

void AddLevelToDoc(Form_V& doc, EL_V* level) {
    if (!level) return;
    if (!doc.head) {
        doc.head = level;
    }
    else {
        EL_V* current = doc.head;
        while (current->next) {
            current = current->next;
        }
        current->next = level;
    }
}

void InitBlock(EL_Stroka*& block, BlockType type) {
    block = new EL_Stroka;
    if (!block) return;
    block->type = type;
    block->sentence_id = 0;
    block->next = nullptr;
    block->next_word_block = nullptr;
    if (type == LETTERS) {
        block->content.letters.size = 0;
        block->content.letters.is_word_part = false;
        for (int i = 0; i < 6; i++) {
            block->content.letters.data[i] = '\0';
        }
    }
    else if (type == SPACES) {
        block->content.spaces.count = 1;
    }
    else if (type == COMMA) {
        block->content.comma.comma = ',';
    }
    else if (type == PUNCTUATION) {
        block->content.punctuation.symbol = '\0';
    }
}

void AddBlockToLine(Line& line, EL_Stroka* block) {
    if (!block) return;
    if (!line.head) {
        line.head = block;
    }
    else {
        EL_Stroka* current = line.head;
        while (current->next) {
            current = current->next;
        }
        current->next = block;
    }
}