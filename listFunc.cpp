#include "listFunc.h"
#include "list.h"

void GetFullWord(EL_Stroka* block, char* result) {
    if (!block || !result) {
        if (result) result[0] = '\0';
        return;
    }
    int pos = 0;
    EL_Stroka* current = block;
    while (current && pos < 255) {
        int i = 0;
        while (i < current->content.letters.size && pos < 255) {
            result[pos++] = current->content.letters.data[i++];
        }
        current = current->next_word_block;
    }
    result[pos] = '\0';
}

void CompareStrings(const char* str1, const char* str2, int& result) {
    result = 0;
    if (!str1 || !str2) {
        result = (str1 ? 1 : (str2 ? -1 : 0));
        return;
    }
    while (*str1 && *str2) {
        if (*str1 != *str2) {
            result = *str1 - *str2;
            return;
        }
        str1++;
        str2++;
    }
    if (*str1 != *str2) {
        result = *str1 ? 1 : -1;
    }
}

void CopyString(char* dest, const char* src, int maxLen) {
    if (!dest || !src) return;
    int i = 0;
    while (i < maxLen - 1 && src[i]) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

bool IsSubstring(const char* str, const char* substr) {
    if (!str || !substr) return false;
    if (!*substr) return true; 

    while (*str) {
        const char* s = str;
        const char* sub = substr;
        while (*s && *sub && *s == *sub) {
            s++;
            sub++;
        }
        if (!*sub) return true; 
        str++;
    }
    return false;
}