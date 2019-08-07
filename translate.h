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
    string name;
    vector<Node*> par_dich;

    CallFunction(string type, vector<Node*> par_dich){
        this->name = name;
        this->par_dich = par_dich;
    }
};

/* TODO: Problemi numerazione linea di errore */

//SymbolTable per tutte le variabili dichiarate
static unordered_multimap<string, Symbol> SymbolTableVar;
//SymbolTable per tutte le variabili dichiarate provvisoria
static unordered_multimap<string, Symbol> SymbolTableVarAux;
//Per parametri passati alla funzione chiamante
static unordered_multimap<string, CallFunction> SymbolTableFunctionCall;
//SymbolTable per tutte le funzioni dichiarate
static unordered_multimap<string, Function> SymbolTableFunction;



//Gestione degli errori
static void Error(string name, int line){
    fprintf(stderr, "line: %d - error: %s \n",line, name.c_str());
    exit(0);
}




