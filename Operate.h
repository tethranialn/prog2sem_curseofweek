#ifndef OPERATE_H
#define OPERATE_H

#include "list.h"
#include <iostream>

void InsertWord(Form_V& doc, const char* targetWord, const char* newWord, int sentenceNum);
void RemoveSpecificPunctuation(Form_V& doc, int sentenceNum, char symbol, std::ostream& out);
void RemoveAllPunctuation(Form_V& doc, int sentenceNum, std::ostream& out);
void PrintOperationResult(const Form_V& doc, const char* targetWord, const char* newWord, int sentenceNum, std::ostream& out);
void DisplayMatchingSentences(const Form_V& doc, const char* word, int* matchingSentences, int& matchCount, std::ostream& detailedOut, std::ostream& conciseOut);

#endif