#include "list.h"
#include "listFunc.h"
#include <iostream>

void InitELStroka(EL_Stroka*& el, const char* value) 
{
    el = new EL_Stroka;
    CopyString(el->A, value, N);
    el->next = nullptr;
}

void InitFormStroka(Form_Stroka& fs) 
{
    fs.h = fs.l = nullptr;
    fs.L = 0;
}

void InitELV(EL_V*& el) 
{
    el = new EL_V;
    InitFormStroka(el->F);
    el->next = nullptr;
}

void InitFormV(Form_V& fv) 
{
    fv.h = fv.cur = fv.last = nullptr;
}

void InsertString(Form_Stroka& fs, const char* value) 
{
    EL_Stroka* newEl;
    InitELStroka(newEl, value);

    if (!fs.h)
    {
        fs.h = fs.l = newEl;
        fs.L = 1;
        return;
    }

    fs.l->next = newEl;
    fs.l = newEl;
    fs.L++;
}
