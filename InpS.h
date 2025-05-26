#ifndef INPS_H
#define INPS_H

#include "list.h"

void ReadDocument(Form_V& doc, const char* filename);
void PrintDocument(const Form_V& doc, std::ostream& out);

#endif