#include "InpS.h"
#include "Del.h"
#include "Operate.h"
#include <fstream>
#include <iostream>

int main() {
    Form_V document;
    ReadDocument(document, "input.txt");

    std::ofstream fout("output.txt");
    if (!fout) {
        std::cerr << "Error creating output file!" << std::endl;
        return 1;
    }

    PrintDocument(document, fout);
    int sentenceNum;
    char targetWord[100], newWord[100];

    std::cout << "Enter sentence number: ";
    std::cin >> sentenceNum;
    std::cout << "Enter target word: ";
    std::cin >> targetWord;
    std::cout << "Enter new word: ";
    std::cin >> newWord;

    InsertWord(document, targetWord, newWord, sentenceNum);

    PrintOperationResult(document, targetWord, newWord, sentenceNum, fout);
    fout.close();

    DeleteDocument(document);

    std::cout << "Processing completed. Results saved to output.txt" << std::endl;
    return 0;
}