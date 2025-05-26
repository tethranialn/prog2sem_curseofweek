#ifndef OPERATE_H
#define OPERATE_H

#include "list.h"
#include "InpS.h"

void InsertWord(Form_V& doc, const char* targetWord, const char* newWord, int sentenceNum);
void PrintOperationResult(const Form_V& doc, const char* targetWord, const char* newWord, int sentenceNum, std::ostream& out);

#endif