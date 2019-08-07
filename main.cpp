#include <iostream>
#include "node.h"
#include "translate.h"

using namespace std;

extern int yyparse();
extern Node* programBlock;


int main ()
{
    yyparse();

    if (auto ExternalDec = programBlock)
    {
        ExternalDec->semantics();
    }else{
        exit(0);
    }

    return 0;
}
