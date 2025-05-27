#ifndef LISTFUNC_H
#define LISTFUNC_H
#include "list.h"

void GetFullWord(EL_Stroka* block, char* result);
void CompareStrings(const char* str1, const char* str2, int& result);
void CopyString(char* dest, const char* src, int maxLen);
bool IsSubstring(const char* str, const char* substr);

#endif