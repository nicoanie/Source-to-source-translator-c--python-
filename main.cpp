#include <iostream>
#include "node.cpp"
#include <fstream>

ofstream __main;

using namespace std;

extern int yyparse();
extern Node* programBlock;


int main ()
{
    yyparse();

    if (auto ExternalDec = programBlock)
    {
        ExternalDec->semantics();

        __main.open("aux.py");
        __main << "#Source generato dal frontend C->Python"<<endl;
        __main.close();

        ExternalDec->translate();
        __main.open("python.py", ios::app);
        __main << endl;
        __main << "main()";
        __main.close();

        ifstream file1( "aux.py" );
        ifstream file2( "python.py" ) ;
        ofstream combined_file( "output.py" ) ;
        combined_file << file1.rdbuf() <<endl<< file2.rdbuf();
    }else{
        exit(0);
    }

    return 0;
}
