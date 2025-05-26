#include "Operate.h"
#include "listFunc.h"
#include <iostream>
#include <fstream>

static EL_V* FindSentenceLevel(Form_V& doc, int sentenceNum, EL_Stroka** outBlock) {
    for (EL_V* level = doc.head; level; level = level->next) {
        for (EL_Stroka* block = level->line.head; block; block = block->next) {
            if (block->sentence_id == sentenceNum && block->type == LETTERS && !block->content.letters.is_word_part) {
                *outBlock = block;
                return level;
            }
        }
    }
    return nullptr;
}

static void GetFullWord(const EL_Stroka* block, char* buffer) {
    int idx = 0;
    while (block && block->type == LETTERS) {
        for (int i = 0; i < block->content.letters.size; i++) {
            buffer[idx++] = block->content.letters.data[i];
        }
        block = block->next_word_block;
    }
    buffer[idx] = '\0';
}

void InsertWord(Form_V& doc, const char* targetWord, const char* newWord, int sentenceNum) {
    EL_Stroka* sentenceStart = nullptr;
    EL_V* targetLevel = FindSentenceLevel(doc, sentenceNum, &sentenceStart);

    if (!targetLevel || !sentenceStart) {
        std::cerr << "Error: Sentence " << sentenceNum << " not found!" << std::endl;
        return;
    }

    EL_Stroka* prevBlock = nullptr;
    EL_Stroka* currentBlock = targetLevel->line.head;
    bool wordFound = false;

    while (currentBlock) {
        if (currentBlock->type == LETTERS && !currentBlock->content.letters.is_word_part) {
            char currentWord[256];
            GetFullWord(currentBlock, currentWord);
            if (CompareStrings(currentWord, targetWord) == 0) {
                wordFound = true;
                break;
            }
        }
        prevBlock = currentBlock;
        currentBlock = currentBlock->next;
    }

    if (!wordFound) {
        std::cerr << "Error: Word '" << targetWord << "' not found in sentence " << sentenceNum << "!" << std::endl;
        return;
    }

    EL_Stroka* spaceAfter;
    InitBlock(spaceAfter, SPACES);
    spaceAfter->content.spaces.count = 1;
    spaceAfter->sentence_id = sentenceNum;

    EL_Stroka* firstNewBlock = nullptr;
    EL_Stroka* lastNewBlock = nullptr;
    EL_Stroka* currentWordPart = nullptr;
    int len = 0;
    while (newWord[len]) len++;

    for (int i = 0; i < len; i += 5) {
        EL_Stroka* part;
        InitBlock(part, LETTERS);
        part->sentence_id = sentenceNum;
        part->content.letters.is_word_part = (i != 0);
        part->content.letters.size = 0;
        for (int j = 0; j < 5 && newWord[i + j]; j++) {
            part->content.letters.data[j] = newWord[i + j];
            part->content.letters.size++;
        }
        part->content.letters.data[part->content.letters.size] = '\0';

        if (!firstNewBlock) {
            firstNewBlock = part;
            currentWordPart = part;
        }
        else {
            lastNewBlock->next = part;              
            currentWordPart->next_word_block = part; 
            currentWordPart = part;
        }
        lastNewBlock = part;
    }

    if (lastNewBlock) {
        lastNewBlock->next = spaceAfter;
    }
    else {
        firstNewBlock = spaceAfter; 
    }
    spaceAfter->next = currentBlock;

    if (prevBlock) {
        prevBlock->next = firstNewBlock;
    }
    else {
        targetLevel->line.head = firstNewBlock;
    }

    targetLevel->line.count += (len / 5 + (len % 5 ? 1 : 0)) + 1; 
}
void PrintOperationResult(const Form_V& doc, const char* targetWord, const char* newWord, int sentenceNum, std::ostream& out) {
    
    out << "\n=== Operation Details ===\n";
    out << "Target sentence: " << sentenceNum << "\n";
    out << "Target word: " << targetWord << "\n";
    out << "New word: " << newWord << "\n";

    out << "\n=== Modified Document ===\n";
    PrintDocument(doc, out);
}
void RemoveSpecificPunctuation(Form_V& doc, int sentenceNum, char symbol, std::ostream& out) {
    bool found = false;
    for (EL_V* level = doc.head; level; level = level->next) {
        EL_Stroka* prev = nullptr;
        EL_Stroka* current = level->line.head;
        while (current) {
            bool shouldDelete = false;
            if (current->sentence_id == sentenceNum) {
                if (current->type == PUNCTUATION && current->content.punctuation.symbol == symbol) {
                    shouldDelete = true;
                }
                else if (current->type == COMMA && symbol == ',') {
                    shouldDelete = true;
                }
            }

            if (shouldDelete) {
                // Удаляем блок
                if (prev) {
                    prev->next = current->next;
                }
                else {
                    level->line.head = current->next;
                }
                if (current == level->line.tail) {
                    level->line.tail = prev;
                }
                EL_Stroka* temp = current;
                current = current->next;
                delete temp;
                level->line.count--;
                found = true;
            }
            else {
                prev = current;
                current = current->next;
            }
        }
    }
    out << "\n=== Removed '" << symbol << "' ===\n";
    PrintDocument(doc, out);
    if (!found) out << "Symbol not found in sentence " << sentenceNum << "!\n";
}
// Удаление всех знаков пунктуации
void RemoveAllPunctuation(Form_V& doc, int sentenceNum, std::ostream& out) {
    int count = 0;
    for (EL_V* level = doc.head; level; level = level->next) {
        EL_Stroka* prev = nullptr;
        EL_Stroka* current = level->line.head;
        while (current) {
            if (current->sentence_id == sentenceNum &&
                (current->type == PUNCTUATION || current->type == COMMA)) {

                // Удаление блока
                if (prev) prev->next = current->next;
                else level->line.head = current->next;

                if (current == level->line.tail) level->line.tail = prev;

                EL_Stroka* temp = current;
                current = current->next;
                delete temp;
                level->line.count--;
                count++;
            }
            else {
                prev = current;
                current = current->next;
            }
        }
    }
    out << "\n=== Remove All Punctuation ===\n";
    PrintDocument(doc, out);
    out << "Removed: " << count << " symbols\n";
}

// Поиск предложения по последнему слову
void FindSentenceByLastWord(const Form_V& doc, const char* word, std::ostream& out) {
    out << "\n=== Sentences ending with \"" << word << "\" ===\n";
    bool found = false;
    for (EL_V* level = doc.head; level; level = level->next) {
        EL_Stroka* block = level->line.head;
        int currentSentence = -1;
        char lastWord[256] = { 0 };

        while (block) {
            if (block->sentence_id != currentSentence) {
                // Проверка предыдущего предложения
                if (currentSentence != -1 && CompareStrings(lastWord, word) == 0) {
                    out << "Sentence " << currentSentence << ":\n";
                    PrintDocument(doc, out);
                    found = true;
                }
                currentSentence = block->sentence_id;
                lastWord[0] = '\0';
            }

            // Сбор слова
            if (block->type == LETTERS && !block->content.letters.is_word_part) {
                EL_Stroka* part = block;
                while (part) {
                    strncat_s(lastWord, part->content.letters.data, part->content.letters.size);
                    part = part->next_word_block;
                }
            }

            block = block->next;
        }
    }
    if (!found) out << "No matches found.\n";
}