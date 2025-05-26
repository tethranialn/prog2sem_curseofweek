#include "Del.h"

void DeleteDocument(Form_V& doc) {
    EL_V* current_level = doc.head;
    while (current_level) {
        EL_Stroka* block = current_level->line.head;
        while (block) {
            EL_Stroka* temp = block;
            block = block->next;
            delete temp;
        }
        EL_V* temp_level = current_level;
        current_level = current_level->next;
        delete temp_level;
    }
    doc.head = doc.tail = nullptr;
    doc.total_sentences = 0;
}