#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <list>

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

string Library[2];

//SymbolTable per tutte le variabili dichiarate
static unordered_multimap<string, Symbol> SymbolTableVar;
//SymbolTable per tutte le funzioni dichiarate
static unordered_multimap<string, Function> SymbolTableFunction;

//Gestione degli errori
static void Error(string name, int line){
    fprintf(stderr, "line: %d - error: %s \n",line, name.c_str());
    exit(0);
}

//Gestione dei warning
static void Warning(string name, int line){
    fprintf(stderr, "line: %d - warning: %s \n",line, name.c_str());
}


void DeclarationVarArgs(string name_func, string name_arg, string type, int line){

    //Trovo in SymbolTableVar tutti i valori appartenenti a quella chiave(il nome della variabile da verificare)
    auto it= SymbolTableVar.find(name_arg);
    for(int i=0; i<SymbolTableVar.count(name_arg); i++){
        if(it->second.scope == name_func){
            if(it->second.type != type)
                //Se il tipo della variabile già memorizzata è diverso da quello della variabile che sto controllando
                Error("Conflicting types.", line);
            else
                //Se il tipo della variabile già memorizzata è uguale a quello della variabile che sto controllando
                Error("Variable already defined.", line);
        }
        it++;
    }
    //Memorizzo la variabile in SymbolTableVar
    //Memorizzo in var il tipo della variabile e il nome della funzione di appartenenza
    Symbol var (type, name_func);
    //Inserisco nella SymbolTable la variabile
    SymbolTableVar.insert({name_arg, var}); //it->second.scope contiene il nome della variabile ottenuto in VariableDec
}

string Type_Expr(int node, int type, int aux_type){
    if(node == 1) return "int";
    if(node == 2) return "float";
    if(node == 3){
        if (type == 1) return "int";
        if (type == 2) return "float";
        if (type == 3) return "char";
        if (type == 4) return "bool";
    }
    if (node == 4) return "char";
    if (node == 5) return "bool";
    if (node == 6){
        if (aux_type == 1) return "int";
        if (aux_type == 2) return "float";
        if (aux_type == 3) return "char";
        if (aux_type == 4) return "bool";
    }
}

string name_function_declaration;
string name_function_call;

int f_c; //Serve ad identifier node per capire quale nodo lo ha richiamato
int iterazione; //Memorizzo il numero di iterazione degli argomenti della funzione che vado a richiamare

float rexpr_value; //Per memorizzare il valore dell'espressione di destra (Utile per l'operatore divisione)

string L; //Per sapere che tipo di variabile sto esaminando a destra dell'operatore
string R; //Per sapere che tipo di variabile sto esaminando a sinistra dell'operatore

int aux_tipo; //Per tenere traccia del tipo di variabile che richiama una funzione

float value_type_node; //Per memorizzare il valore quando si entra nei nodi type
int nodo; /* Per sapere in quale type node è entrato
 *          -1: Integer
 *          -2: Float
 *          -3: Identifier
 *          -4: Char
 *          -5: Bool
 *          -6: FunctionCall
 *          -7: Assignment
 *          -8: While
 *          -9: Binary Operator
 *          -10: If
 *          -11: Print*/
int tipo; /* Per sapere di che tipo è la varibile in esame
 *          -1: Integer
 *          -2: Float
 *          -3: Char
 *          -4: Bool*/

int return_in = 0;