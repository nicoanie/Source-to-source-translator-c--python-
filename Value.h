#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Struttura{
public:
    string name;
    string type;
    int linea;
    string controllo;
    int ALTRIMENTI_NON_PRENDE_ULTIMO_VALORE;

    Struttura(string name, string type, int linea, string controllo, int ALTRIMENTI_NON_PRENDE_ULTIMO_VALORE){
        this->name = name;
        this->type = type;
        this->linea = linea;
        this->controllo = controllo;
        this->ALTRIMENTI_NON_PRENDE_ULTIMO_VALORE=ALTRIMENTI_NON_PRENDE_ULTIMO_VALORE;
    }

};
