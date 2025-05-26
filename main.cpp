#include "InpS.h"
#include "Del.h"
#include "Operate.h"
#include <fstream>
#include <iostream>

void ShowMenu() {
    system("cls");
    std::cout << "\n======= Menu =======\n"
        << "1. Insert word\n"
        << "2. Remove specific punctuation\n"
        << "3. Remove all punctuation\n"
        << "4. Find sentence by last word\n"
        << "5. Exit\n"
        << "====================\n"
        << "Choose option: ";
}

int main() {
    Form_V document;
    ReadDocument(document, "input.txt");
    std::ofstream fout("output.txt", std::ofstream::trunc);
    PrintDocument(document, fout);
    do {
        int choice;
        ShowMenu();
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
        case 1: {
            int sentenceNum;
            char buffer[100];
            std::cout << "Enter sentence number: ";
            std::cin >> sentenceNum;
            std::cout << "Enter target word: ";
            std::cin >> buffer;
            char newWord[100];
            std::cout << "Enter new word: ";
            std::cin >> newWord;
            InsertWord(document, buffer, newWord, sentenceNum);
            PrintOperationResult(document, buffer, newWord, sentenceNum, fout);
            break;
        }
        case 2: {
            int sentenceNum;
            char symbol;
            std::cout << "Enter sentence number: ";
            std::cin >> sentenceNum;
            std::cout << "Enter symbol: ";
            std::cin >> symbol;
            RemoveSpecificPunctuation(document, sentenceNum, symbol, fout);
            break;
        }
        case 3: {
            int sentenceNum;
            std::cout << "Enter sentence number: ";
            std::cin >> sentenceNum;
            RemoveAllPunctuation(document, sentenceNum, fout);
            break;
        }
        case 4: {
            char buffer[100];
            std::cout << "Enter last word: ";
            std::cin >> buffer;
            FindSentenceByLastWord(document, buffer, fout);
            break;
        }
        case 5: {
            std::cout << "Exiting...\n";
            DeleteDocument(document);
            fout.close();
            return 0;
        }
        default:
        {
            std::cout << "Invalid option!\n";
            break;
        }
        }
    } while (12);

    DeleteDocument(document);
    fout.close();
    return 0;
}