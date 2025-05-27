#include "Del.h"
#include "list.h"

void DeleteDocument(Form_V& doc) {
    EL_V* level = doc.head;
    while (level) {
        EL_Stroka* block = level->line.head;
        while (block) {
            EL_Stroka* nextBlock = block->next;
            EL_Stroka* wordPart = block->next_word_block;
            while (wordPart) {
                EL_Stroka* nextPart = wordPart->next_word_block;
                delete wordPart;
                wordPart = nextPart;
            }
            delete block;
            block = nextBlock;
        }
        EL_V* nextLevel = level->next;
        delete level;
        level = nextLevel;
    }
    doc.head = nullptr;
    doc.total_sentences = 0;
}