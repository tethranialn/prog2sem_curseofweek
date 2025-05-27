#include "Operate.h"
#include "list.h"
#include "listFunc.h"
#include "inpS.h"
#include <fstream>
#include <iostream>

void InsertWord(Form_V& doc, const char* targetWord, const char* newWord, int sentenceNum) {
    if (!targetWord || !newWord || !doc.head) return;

    int cmpResult = 0;
    bool insertBeforeAll = false;
    CompareStrings(targetWord, "all", cmpResult);
    if (cmpResult == 0) insertBeforeAll = true;

    EL_V* level = doc.head;
    while (level) {
        if (!level->line.head) {
            level = level->next;
            continue;
        }

        EL_Stroka* block = level->line.head;
        EL_Stroka* prev = nullptr;

        while (block) {
            EL_Stroka* nextBlock = block ? block->next : nullptr;
            if (!block || block->sentence_id != sentenceNum) {
                prev = block;
                block = nextBlock;
                continue;
            }

            if (block->type == LETTERS && !block->content.letters.is_word_part) {
                char currentWord[256] = { 0 };
                GetFullWord(block, currentWord);

                bool shouldInsert = false;
                if (insertBeforeAll) {
                    shouldInsert = true;
                }
                else {
                    if (IsSubstring(currentWord, targetWord)) {
                        shouldInsert = true;
                    }
                }

                if (shouldInsert) {
                    EL_Stroka* newBlock = nullptr;
                    InitBlock(newBlock, LETTERS);
                    if (!newBlock) {
                        std::cerr << "Error: Failed to allocate memory for new word block.\n";
                        return;
                    }
                    newBlock->sentence_id = sentenceNum;

                    int i = 0;
                    while (newWord[i] && i < 5) {
                        newBlock->content.letters.data[i] = newWord[i];
                        newBlock->content.letters.size++;
                        i++;
                    }
                    newBlock->content.letters.data[i] = '\0';

                    const char* remaining = newWord + i;
                    EL_Stroka* currentPart = newBlock;
                    while (*remaining) {
                        EL_Stroka* partBlock = nullptr;
                        InitBlock(partBlock, LETTERS);
                        if (!partBlock) {
                            std::cerr << "Error: Failed to allocate memory for word part block.\n";
                            EL_Stroka* temp = newBlock;
                            while (temp) {
                                EL_Stroka* next = temp->next_word_block;
                                delete temp;
                                temp = next;
                            }
                            return;
                        }
                        partBlock->content.letters.is_word_part = true;
                        partBlock->sentence_id = sentenceNum;
                        i = 0;
                        while (*remaining && i < 5) {
                            partBlock->content.letters.data[i] = *remaining;
                            partBlock->content.letters.size++;
                            remaining++;
                            i++;
                        }
                        partBlock->content.letters.data[i] = '\0';
                        currentPart->next_word_block = partBlock;
                        currentPart = partBlock;
                    }

                    EL_Stroka* spaceAfter = nullptr;
                    InitBlock(spaceAfter, SPACES);
                    if (!spaceAfter) {
                        std::cerr << "Error: Failed to allocate memory for space after block.\n";
                        EL_Stroka* temp = newBlock;
                        while (temp) {
                            EL_Stroka* next = temp->next_word_block;
                            delete temp;
                            temp = next;
                        }
                        return;
                    }
                    spaceAfter->sentence_id = sentenceNum;
                    spaceAfter->content.spaces.count = 1;

                    newBlock->next = spaceAfter;
                    spaceAfter->next = block;

                    if (prev) {
                        prev->next = newBlock;
                    }
                    else {
                        level->line.head = newBlock;
                    }
                    prev = spaceAfter;
                }
                else {
                    prev = block;
                }
            }
            else {
                prev = block;
            }
            block = nextBlock;
        }
        level = level->next;
    }
}

void RemoveSpecificPunctuation(Form_V& doc, int sentenceNum, char symbol, std::ostream& out) {
    for (EL_V* level = doc.head; level; level = level->next) {
        if (!level || !level->line.head) continue;
        EL_Stroka* block = level->line.head;
        EL_Stroka* prev = nullptr;

        while (block) {
            EL_Stroka* nextBlock = block->next;
            if (block->sentence_id != sentenceNum) {
                prev = block;
                block = nextBlock;
                continue;
            }

            if ((block->type == PUNCTUATION && block->content.punctuation.symbol == symbol) ||
                (block->type == COMMA && block->content.comma.comma == symbol)) {
                if (prev) {
                    prev->next = block->next;
                }
                else {
                    level->line.head = block->next;
                }
                delete block;
                block = nextBlock;
                continue;
            }
            prev = block;
            block = nextBlock;
        }
    }
    out << "\n=== Remove Specific Punctuation Operation ===\n";
    out << "Removed symbol: " << symbol << "\nSentence: " << sentenceNum << "\n";
    PrintDocument(doc, out);
}

void RemoveAllPunctuation(Form_V& doc, int sentenceNum, std::ostream& out) {
    for (EL_V* level = doc.head; level; level = level->next) {
        if (!level || !level->line.head) continue;
        EL_Stroka* block = level->line.head;
        EL_Stroka* prev = nullptr;

        while (block) {
            EL_Stroka* nextBlock = block->next;
            if (block->sentence_id != sentenceNum) {
                prev = block;
                block = nextBlock;
                continue;
            }

            if (block->type == PUNCTUATION || block->type == COMMA) {
                if (prev) {
                    prev->next = block->next;
                }
                else {
                    level->line.head = block->next;
                }
                delete block;
                block = nextBlock;
                continue;
            }
            prev = block;
            block = nextBlock;
        }
    }
    out << "\n=== Remove All Punctuation Operation ===\n";
    out << "Sentence: " << sentenceNum << "\n";
    PrintDocument(doc, out);
}

void PrintOperationResult(const Form_V& doc, const char* targetWord, const char* newWord, int sentenceNum, std::ostream& out) {
    out << "Inserted word \"" << newWord << "\" before \"" << targetWord << "\" in sentence " << sentenceNum << "\n";
    PrintDocument(doc, out);
}