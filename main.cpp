#include "listFunc.h"
#include "InpS.h"
#include "Del.h"
#include "Operate.h"
#include <fstream>
#include <iostream>

void ShowMenu() {
    std::cout << "\n======= Menu =======\n"
        << "1. Insert word in matching sentence\n"
        << "2. Remove specific punctuation in matching sentence\n"
        << "3. Remove all punctuation in matching sentence\n"
        << "4. Find new sentences by last word\n"
        << "5. Exit\n"
        << "====================\n"
        << "Choose option: ";
}

void DisplayMatchingSentences(const Form_V& doc, const char* word, int* matchingSentences, int& matchCount, std::ostream& detailedOut, std::ostream& conciseOut) {
    detailedOut << "\n=== Sentences ending with \"" << word << "\" ===\n";
    conciseOut << "\n=== Sentences ending with \"" << word << "\" ===\n";
    std::cout << "\n=== Sentences ending with \"" << word << "\" ===\n";
    std::cout << std::flush;
    matchCount = 0;

    char lastWord[256] = { 0 };
    int currentSentence = -1;
    EL_Stroka* lastWordBlock = nullptr;
    bool found = false;

    for (EL_V* level = doc.head; level; level = level->next) {
        if (!level || !level->line.head) continue;
        EL_Stroka* block = level->line.head;
        while (block) {
            if (block->sentence_id != currentSentence) {
                if (currentSentence != -1 && lastWordBlock && lastWord[0] != '\0') {
                    int cmpResult = 0;
                    CompareStrings(lastWord, word, cmpResult);
                    if (cmpResult == 0) {
                        if (matchCount < 100) {
                            matchingSentences[matchCount++] = currentSentence;
                        }
                        detailedOut << "Sentence " << currentSentence << ":\n";
                        conciseOut << "Sentence " << currentSentence << ": ";
                        std::cout << "Sentence " << currentSentence << ": ";

                        // Обход всех блоков в предложении для вывода
                        for (EL_V* printLevel = doc.head; printLevel; printLevel = printLevel->next) {
                            EL_Stroka* printBlock = printLevel->line.head;
                            while (printBlock) {
                                if (printBlock->sentence_id == currentSentence) {
                                    if (printBlock->type == LETTERS) {
                                        // Вывод всех частей слова
                                        EL_Stroka* wordPart = printBlock;
                                        while (wordPart) {
                                            std::cout << wordPart->content.letters.data;
                                            conciseOut << wordPart->content.letters.data;
                                            detailedOut << "  [WORD: \"" << wordPart->content.letters.data << "\"]\n";
                                            wordPart = wordPart->next_word_block;
                                        }
                                    }
                                    else if (printBlock->type == SPACES) {
                                        for (int i = 0; i < printBlock->content.spaces.count; i++) {
                                            std::cout << ' ';
                                            conciseOut << ' ';
                                        }
                                    }
                                    else if (printBlock->type == COMMA || printBlock->type == PUNCTUATION) {
                                        char symbol = (printBlock->type == COMMA) ? printBlock->content.comma.comma
                                            : printBlock->content.punctuation.symbol;
                                        std::cout << symbol;
                                        conciseOut << symbol;
                                    }
                                }
                                printBlock = printBlock->next;
                            }
                        }
                        std::cout << "\n";
                        conciseOut << "\n";
                        detailedOut << "----------------\n";
                        found = true;
                    }
                }
                currentSentence = block->sentence_id;
                lastWord[0] = '\0';
                lastWordBlock = nullptr;
            }

            if (block->type == LETTERS && !block->content.letters.is_word_part) {
                lastWordBlock = block;
                GetFullWord(block, lastWord);
            }

            block = block->next;
        }
    }

    // Проверка последнего предложения
    if (currentSentence != -1 && lastWordBlock && lastWord[0] != '\0') {
        int cmpResult = 0;
        CompareStrings(lastWord, word, cmpResult);
        if (cmpResult == 0) {
            if (matchCount < 100) {
                matchingSentences[matchCount++] = currentSentence;
            }
            detailedOut << "Sentence " << currentSentence << ":\n";
            conciseOut << "Sentence " << currentSentence << ": ";
            std::cout << "Sentence " << currentSentence << ": ";

            // Вывод последнего предложения
            for (EL_V* printLevel = doc.head; printLevel; printLevel = printLevel->next) {
                EL_Stroka* printBlock = printLevel->line.head;
                while (printBlock) {
                    if (printBlock->sentence_id == currentSentence) {
                        if (printBlock->type == LETTERS) {
                            EL_Stroka* wordPart = printBlock;
                            while (wordPart) {
                                std::cout << wordPart->content.letters.data;
                                conciseOut << wordPart->content.letters.data;
                                detailedOut << "  [WORD: \"" << wordPart->content.letters.data << "\"]\n";
                                wordPart = wordPart->next_word_block;
                            }
                        }
                        else if (printBlock->type == SPACES) {
                            for (int i = 0; i < printBlock->content.spaces.count; i++) {
                                std::cout << ' ';
                                conciseOut << ' ';
                            }
                        }
                        else if (printBlock->type == COMMA || printBlock->type == PUNCTUATION) {
                            char symbol = (printBlock->type == COMMA) ? printBlock->content.comma.comma
                                : printBlock->content.punctuation.symbol;
                            std::cout << symbol;
                            conciseOut << symbol;
                        }
                    }
                    printBlock = printBlock->next;
                }
            }
            std::cout << "\n";
            conciseOut << "\n";
            detailedOut << "----------------\n";
            found = true;
        }
    }

    if (!found) {
        detailedOut << "No sentences ending with \"" << word << "\" found.\n";
        conciseOut << "No sentences ending with \"" << word << "\" found.\n";
        std::cout << "No sentences ending with \"" << word << "\" found.\n" << std::flush;
    }
    else {
        std::cout << "Matching sentences: ";
        for (int i = 0; i < matchCount; i++) {
            std::cout << matchingSentences[i] << " ";
        }
        std::cout << "\n" << std::flush;
    }
}
int main() {
    Form_V document;
    std::ofstream detailedOut("output_detailed.txt", std::ofstream::trunc);
    std::ofstream conciseOut("output_concise.txt", std::ofstream::trunc);

    if (!detailedOut.is_open() || !conciseOut.is_open()) {
        std::cerr << "Error: Unable to open output files!\n";
        return 1;
    }

    ReadDocument(document, "input.txt");
    if (!document.head) {
        std::cerr << "Error: Failed to read input file or file is empty!\n";
        detailedOut.close();
        conciseOut.close();
        return 1;
    }

    detailedOut << "=== Initial Document ===\n";
    PrintDocument(document, detailedOut);
    conciseOut << "=== Initial Document ===\n";
    PrintDocumentConcise(document, conciseOut);

    int matchingSentences[100];
    int matchCount = 0;
    char lastWord[100] = { 0 };

    while (true) {
        std::cout << "\nEnter last word to find sentences (or 'exit' to quit, max 99 characters): ";
        std::cout << std::flush;
        std::cin.getline(lastWord, 100);
        if (std::cin.fail()) {
            std::cerr << "Error: Invalid input. Please try again.\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }
        int len = 0;
        while (lastWord[len]) len++;
        while (len > 0 && (lastWord[len - 1] == ' ' || lastWord[len - 1] == '\n')) {
            lastWord[len - 1] = '\0';
            len--;
        }
        int cmpResult = 0;
        CompareStrings(lastWord, "exit", cmpResult);
        if (cmpResult == 0) {
            std::cout << "Exiting...\n" << std::flush;
            conciseOut << "\n=== Final Document ===\n";
            PrintDocumentConcise(document, conciseOut);
            DeleteDocument(document);
            detailedOut.close();
            conciseOut.close();
            return 0;
        }

        DisplayMatchingSentences(document, lastWord, matchingSentences, matchCount, detailedOut, conciseOut);
        if (matchCount == 0) {
            std::cout << "Please try another word.\n" << std::flush;
            continue;
        }

        while (true) {
            ShowMenu();
            std::cout << std::flush;
            int choice;
            if (!(std::cin >> choice)) {
                std::cerr << "Invalid input! Please enter a number.\n";
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                continue;
            }
            std::cin.ignore();

            if (choice == 4) {
                break;
            }

            if (choice == 5) {
                std::cout << "Exiting...\n" << std::flush;
                conciseOut << "\n=== Final Document ===\n";
                PrintDocumentConcise(document, conciseOut);
                DeleteDocument(document);
                detailedOut.close();
                conciseOut.close();
                return 0;
            }

            if (choice < 1 || choice > 5) {
                std::cout << "Invalid option!\n" << std::flush;
                continue;
            }

            int sentenceNum;
            std::cout << "Enter sentence number (from matching sentences: ";
            for (int i = 0; i < matchCount; i++) {
                std::cout << matchingSentences[i] << " ";
            }
            std::cout << "): " << std::flush;
            if (!(std::cin >> sentenceNum)) {
                std::cerr << "Invalid sentence number!\n";
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                continue;
            }
            std::cin.ignore();

            bool validSentence = false;
            for (int i = 0; i < matchCount; i++) {
                if (matchingSentences[i] == sentenceNum) {
                    validSentence = true;
                    break;
                }
            }
            if (!validSentence) {
                std::cerr << "Error: Sentence " << sentenceNum << " is not in the matching list!\n";
                continue;
            }

            switch (choice) {
            case 1: {
                char targetWord[100];
                std::cout << "Enter target word (or 'all' for all words, max 99 characters): " << std::flush;
                std::cin.getline(targetWord, 100);
                if (std::cin.fail()) {
                    std::cerr << "Error: Invalid input. Please try again.\n";
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    continue;
                }
                int targetLen = 0;
                while (targetWord[targetLen]) targetLen++;
                while (targetLen > 0 && (targetWord[targetLen - 1] == ' ' || targetWord[targetLen - 1] == '\n')) {
                    targetWord[targetLen - 1] = '\0';
                    targetLen--;
                }
                char newWord[100];
                std::cout << "Enter new word (max 99 characters): " << std::flush;
                std::cin.getline(newWord, 100);
                if (std::cin.fail()) {
                    std::cerr << "Error: Invalid input. Please try again.\n";
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    continue;
                }
                int newLen = 0;
                while (newWord[newLen]) newLen++;
                while (newLen > 0 && (newWord[newLen - 1] == ' ' || newWord[newLen - 1] == '\n')) {
                    newWord[newLen - 1] = '\0';
                    newLen--;
                }
                InsertWord(document, targetWord, newWord, sentenceNum);
                detailedOut << "\n=== Insert Word Operation ===\n";
                PrintOperationResult(document, targetWord, newWord, sentenceNum, detailedOut);
                break;
            }
            case 2: {
                char symbol;
                std::cout << "Enter punctuation symbol to remove: " << std::flush;
                std::cin >> symbol;
                std::cin.ignore();
                RemoveSpecificPunctuation(document, sentenceNum, symbol, detailedOut);
                detailedOut << "Last word searched: " << lastWord << "\n";
                break;
            }
            case 3: {
                RemoveAllPunctuation(document, sentenceNum, detailedOut);
                detailedOut << "Last word searched: " << lastWord << "\n";
                break;
            }
            }
        }
    }

    DeleteDocument(document);
    detailedOut.close();
    conciseOut.close();
    return 0;
}