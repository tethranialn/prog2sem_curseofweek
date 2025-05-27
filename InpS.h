#ifndef INPS_H
#define INPS_H

#include "list.h"
#include <iostream>
#include <fstream> // Added for std::ifstream

void ReadDocument(Form_V& doc, const char* filename);
void PrintDocument(const Form_V& doc, std::ostream& out);
void PrintDocumentConcise(const Form_V& doc, std::ostream& out);


#endif