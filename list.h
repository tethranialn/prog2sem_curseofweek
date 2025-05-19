#pragma once
#ifndef LIST_H
#define LIST_H
#include <fstream>

const unsigned N = 6;

struct EL_Stroka 
{
    char A[N];
    EL_Stroka* next;
};

struct Form_Stroka 
{
    EL_Stroka* h, * l;
    unsigned L;
};

struct EL_V 
{
    Form_Stroka F;
    EL_V* next;
};

struct Form_V 
{
    EL_V* h, * cur, * last;
};

void InitELStroka(EL_Stroka*& el, const char* value);
void InitFormStroka(Form_Stroka& fs);
void InitELV(EL_V*& el);
void InitFormV(Form_V& fv);
void InsertString(Form_Stroka& fs, const char* value);

#endif