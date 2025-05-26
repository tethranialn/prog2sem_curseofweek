#ifndef OPERATE_H
#define OPERATE_H

#include "list.h"
#include "InpS.h"

void InsertWord(Form_V& doc, const char* targetWord, const char* newWord, int sentenceNum);
void PrintOperationResult(const Form_V& doc, const char* targetWord, const char* newWord, int sentenceNum, std::ostream& out);
void RemoveSpecificPunctuation(Form_V& doc, int sentenceNum, char symbol, std::ostream& out);
void RemoveAllPunctuation(Form_V& doc, int sentenceNum, std::ostream& out);
void FindSentenceByLastWord(const Form_V& doc, const char* word, std::ostream& out);


#endif