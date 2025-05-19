#include "listFunc.h"

void CopyString(char* dest, const char* src, unsigned max) {
    unsigned i = 0;
    while (i < max - 1 && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

int CompareStrings(const char* s1, const char* s2) {
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0') {
        if (s1[i] < s2[i]) return -1;
        if (s1[i] > s2[i]) return 1;
        i++;
    }
    if (s1[i] == '\0' && s2[i] != '\0') return -1;
    if (s1[i] != '\0' && s2[i] == '\0') return 1;
    return 0;
}