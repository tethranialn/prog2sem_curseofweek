#include "InpS.h"
#include "Del.h"
#include <fstream>
#include <iostream>

int main()
{
    Form_V document;

    ReadDocument(document, "input.txt");

    std::ofstream fout("output.txt");
    if (!fout) {
        std::cerr << "Error creating output file!" << std::endl;
        return 1;
    }
    PrintDocument(document, fout);
    fout.close();

    DeleteDocument(document);

    std::cout << "Program completed successfully. Results saved to output.txt" << std::endl;

    return 0;
}