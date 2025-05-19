#include "Del.h"
#include "list.h"

void Delete(Form_V& fv) {
    EL_V* curV = fv.h;
    while (curV) {
        EL_Stroka* curS = curV->F.h;
        while (curS) {
            EL_Stroka* tempS = curS;
            curS = curS->next;
            delete tempS;
        }
        EL_V* tempV = curV;
        curV = curV->next;
        delete tempV;
    }
    InitFormV(fv);
}