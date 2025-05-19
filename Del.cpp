#include "Del.h"
#include <iostream>

void DeleteDocument(Form_V& doc) {
    EL_V* current_level = doc.head;
    while (current_level) {
        EL_Stroka* current_block = current_level->line.head;
        while (current_block) {
            EL_Stroka* block_to_delete = current_block;
            current_block = current_block->next;

            if (block_to_delete->type == LETTERS) {
                delete[] block_to_delete->content.letters.data;
            }

            delete block_to_delete;
        }
        EL_V* level_to_delete = current_level;
        current_level = current_level->next;
        delete level_to_delete;
    }
    doc.head = doc.tail = nullptr;
    doc.total_sentences = 0;
}