#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

//mappa contenente le variabili istanziate all'interno di ogni funzione

class Symbol {
public:
    string type;
    string scope;

    Symbol(string type, string scope){
        this->type = type;
        this->scope = scope;
    }

};

class SymbolAux {
public:
    string type;
    string name;
    int line;

    SymbolAux(string type, string name, int line){
        this->type = type;
        this->name = name;
        this->line = line;
    }

};

class Function {
public:
    string type;
    vector<Node*> args;

    Function(string type, vector<Node*> args){
        this->type = type;
        this->args = args;
    }

};

class CallFunction{
public:
    string value;
    int line;

    CallFunction(string value, int line){
        this->value = value;
        this->line = line;
    }
};

//SymbolTable per tutte le variabili dichiarate
static unordered_multimap<string, Symbol> SymbolTableVar;
//Per parametri passati alla funzione chiamante
static unordered_multimap<string, CallFunction> FunctionCall;
//SymbolTable per tutte le funzioni dichiarate
static unordered_multimap<string, Function> SymbolTableFunction;



//Gestione degli errori
static void Error(string name, int line){
    fprintf(stderr, "line: %d - error: %s \n",line, name.c_str());
    exit(0);
}

//Gestione degli warning
static void Warning(string name, int line){
    fprintf(stderr, "line: %d - warning: %s \n",line, name.c_str());
}


void DeclarationVarArgs(string name_func, string name_arg, string type, int line){

    //Trovo in SymbolTableVar tutti i valori appartenenti a quella chiave(il nome della variabile da verificare)
    auto it= SymbolTableVar.find(name_arg);
    for(int i=0; i<SymbolTableVar.count(name_arg); i++){
        if(it->second.scope == name_func || it->second.scope == "global")
            if(it->second.type != type)
                //Se il tipo della variabile già memorizzata è diverso da quello della variabile che sto controllando
                Error("Conflicting types.", line);
            else
                //Se il tipo della variabile già memorizzata è uguale a quello della variabile che sto controllando
                Error("Variable already defined.", line);
    }
    //Memorizzo la variabile in SymbolTableVar
    //Memorizzo in var il tipo della variabile e il nome della funzione di appartenenza
    Symbol var (type, name_func);
    //Inserisco nella SymbolTable la variabile
    SymbolTableVar.insert({name_arg, var}); //it->second.scope contiene il nome della variabile ottenuto in VariableDec
}



//TODO: Gestire errori sui tipi di variabili
string name_function_declaration;
string name_function_call;
int f_c; //Serve ad identifier node per capire quale nodo lo ha richiamato
int iterazione; //Memorizzo il numero di iterazione degli argomenti della funzione che vado a richiamare
float lexpr_value; //Per memorizzare il valore dell'espressione di sinistra
float rexpr_value; //Per memorizzare il valore dell'espressione di destra
float value_type_node; //Per memorizzare il valore quando si entra nei nodi type
int nodo; /* Per sapere in quale type node è entrato
 *          -1: Integer
 *          -2: Float
 *          -3: Identifier
 *          -4: Char
 *          -5: Bool
 *          -6: FunctionCall*/
int tipo; /* Per sapere di che tipo è la varibile in esame
 *          -1: Integer
 *          -2: Float
 *          -3: Char
 *          -4: Bool*/