#include "node.cpp"
#include "parser.hpp"
#include "semantics.h"


void ExternalDec :: semantics(){
    cout << "Dentro ExternalDec" << endl;

    //Memorizzo all'interno della symbol table le variabili globali
    for(int i=0; i<variables.size(); i++){
        //Memorizzo nella variabile it le informazioni ricercate nella SymbolTableVar
        auto it = SymbolTableVar.find(variables[i]->name);

        //Se non trovo niente, significa che posso memorizzare la variabile
        if (SymbolTableVar.count(variables[i]->name)==0){
            //Creo una nuova variabile di tipo Symbol contenente il tipo della variabile e la funzione di appartenenza
            Symbol var (variables[i]->type, "global");
            //La memorizzo in SymbolTableVar
            SymbolTableVar.insert({variables[i]->name, var});
        }
        else
            Error("Variable already defined.", line);
    }
/*
    *//*  DEBUG   *//*
    //Verifico se le informazioni vengono salvate bene nella unordered map(SymbolTableVar)
    auto aux = SymbolTableVar.find("a");
    for(int i=0; i<SymbolTableVar.count("a"); i++){
        cout << "Info var current: "<< "nome: " << aux->first << "type: " << aux->second.type << "Scope: "<< aux->second.scope << endl;
        aux++; // Incremento aux, altrimenti resta sempre alla stessa iterazione
    }

    *//*  FINE DEBUG  */


    //Ispeziono tutte le funzioni che sono state dichiarate
    for(int i=0; i<function.size(); i++){
        function[i]->semantics();
    }

    //Verifico che tra le funzioni sia presente una che si chiami main
    if(SymbolTableFunction.count("main") == 0)
        Error("Undefined reference to 'main'.", line);
    else{
        //Verifico che main sia di tipo int o void
        auto it1= SymbolTableFunction.find("main");
        if(it1->second.type != "int" && it1->second.type != "void")
            Error("Wrong function type.", line);
    }
}

void FuncDecl :: semantics(){
    cout << "Dentro FuncDecl" << endl;
    name_function_declaration = name;
    Function func (type, args);

    //Verifico che la funzione non sia già presente all'interno della Symbol table e nel caso la aggiungo
    if(SymbolTableFunction.count(name)==0){
        SymbolTableFunction.insert({name, func});
    }
    else{
        auto it = SymbolTableFunction.find(name);
        if(it->second.type == type)
            Error("Function already defined.",line);
        else
            Error("Conflicting type for function.", line);
    }

    //Memorizzo tutti gli argomenti della funzione nella Symbol table
    for(int i=0; i<args.size(); i++){
        DeclarationVarArgs(name, args[i]->name, args[i]->type, args[i]->line);
    }

    //Ispeziono il corpo della funzione
    for(int i=0; i<body.size(); i++){
        body[i]->semantics();
    }
}

void VariableDec :: semantics(){
    cout << "Dentro VariableDecNode" << endl;
    //Verifico se è già presente una variabile con lo stesso nome
    if(SymbolTableVar.count(name)!=0){
        //Verifico che non sia una variabile globale e non sia già stata assegnata a quella funzione
        auto it = SymbolTableVar.find(name);
        for(int i=0; i< SymbolTableVar.count(name); i++){
            if(it->second.scope == "global" && it->second.type != type )
                Error("Conflicting type for function.", line);
            if(it->second.scope == "global" && it->second.type == type )
                Error("Variable already defined.", line);
            if(it->second.scope == name_function_declaration && it->second.type != type )
                Error("Conflicting type for function.", line);
            if(it->second.scope == name_function_declaration && it->second.type == type )
                Error("Variable already defined.", line);
            it++;
        }
    }

    Symbol var (type, name_function_declaration);
    //Inserisco nella SymbolTable la variabile
    SymbolTableVar.insert({name, var});
}


void FunctionCall :: semantics(){
    cout << "Dentro FunctionCall" << endl;
    name_function_call = name;
    //Se non trovo la funzione deve egnalare errore in quanto non è stata dichiarata
    if(SymbolTableFunction.count(name)==0)
        Error("Implicit declaretion of function.", line);

    //Se la funzione viene trovata verifico che il numero di parametri della funzione dichiarata siano uguali a quelli che passo
    auto it = SymbolTableFunction.find(name);
    if(it->second.args.size() > args.size())
        Error("Too few arguments to function call.", line);
    if(it->second.args.size() < args.size())
        Error("Too many arguments to function call.", line);

    //Se il numero di parametri corrisponde, verifico che siano stati dichiarati e corrispondano allo stesso tipo
    //Vado a gestire i parametri
    for(int i=0; i<args.size(); i++){
        iterazione = i; //Memorizzo il numero dell'iterazione
        //Richiamo il nodo identifier per ogni parametro ed effettuo i controlli
        args[i]->semantics();
    }

/*
    *//*  DEBUG  *//*
    //Verifico se le informazioni vengono salvate bene nella unordered map(SymbolTableVar)
    auto aux = SymbolTableFunctionCall.find("null");
    for(int i=0; i<SymbolTableFunctionCall.count("null"); i++){
        cout << "Info var current: "<< "value: " << aux->second.value << "line: " << aux->second.line << endl;
        aux++; // Incremento aux, altrimenti resta sempre alla stessa iterazione
    }

    *//*  FINE DEBUG  *//*
    */
}

void IdentifierNode :: semantics(){
    cout << "Dentro IdentifierNode" << endl;

    //Verifico che la variabile è stata dichiarata all'interno della funzione
    if(SymbolTableVar.count(value) == 0)
        Error("Variable undeclared (first use in this function).", line);

    //Se è presente nella symbol table devo verificare che appartenga alla funzione o sia globale
    bool found;
    auto it = SymbolTableVar.find(value);

    //Versione controllo dei tipi
    auto it2= SymbolTableFunction.find(name_function_call);
    for(int i=0; i< SymbolTableVar.count(value); i++){
        if(it->second.scope == name_function_declaration || it->second.scope == "global"){
            if(it->second.type != it2->second.args[iterazione]->type)
                Error("Error type variable.", line);
                //TODO: Gestire casting varibili
            found = true; // Corrispondenza variabile trovata
            i=SymbolTableVar.count(value);
        }
        it++;
    }

    if (!found)
        Error("Variable undeclared (first use in this function).", line);
}

void Assignment :: semantics(){

    cout << "Dentro AssignmentNode" << endl;
    //Verifico che la variabile sia stata dichiarata
    bool found;
    auto it = SymbolTableVar.find(name);
    for(int i=0; i<SymbolTableVar.count(name);i++){
        if(it->second.scope == name_function_declaration || it->second.scope == "global"){
            found = true;
            i=SymbolTableVar.count(name);
        }
        it++;
    }
    if (!found)
        Error("Variable undeclared.", line);

    value->semantics();
}


/*--------------------------------------------- TODO: Cose da fare -------------------------------------------------- */

void IntegerNode :: semantics(){
    cout << "Dentro IntegerNode" << endl;
    cout << "TODO:Vedere che cazzo fare" << endl;

    //TODO:Vedere che cazzo fare(dovrei restituire il valore intero)

}

void FloatNode :: semantics(){
    cout << "Dentro FloatNode" << endl;
    cout << "TODO:Vedere che cazzo fare" << endl;

    //TODO:Vedere che cazzo fare(dovrei restituire il valore float)

}

void CharNode :: semantics(){
    cout << "Dentro CharNode" << endl;
    cout << "TODO:Vedere che cazzo fare" << endl;

    //TODO:Vedere che cazzo fare(dovrei restituire il valore char)

}

void BoolNode :: semantics(){
    cout << "Dentro BoolNode" << endl;
    cout << "TODO:Vedere che cazzo fare" << endl;

    //TODO:Vedere che cazzo fare(dovrei restituire il valore 0 o 1)

}


void IfStmt :: semantics(){

    cout << "Dentro IfNode" << endl;
    condition->semantics();
    for(int i=0; i<body_if.size();i++){
        cout << "Body if" << endl;
        body_if[i]->semantics();
    }
    for(int i=0; i<body_else.size(); i++){
        cout << "Body else" << endl;
        body_else[i]->semantics();
    }
}

void WhileStmt :: semantics(){

    cout << "Dentro WhileNode" << endl;
/*    cout <<"Nome : "<< name <<  endl;
*/

}

void ReturnNode :: semantics(){

    cout << "Dentro ReturnNode" << endl;
/*    cout <<"Nome : "<< name <<  endl;
*/

}

void PrintStmt :: semantics(){

    cout << "Dentro PrintNode" << endl;
/*    cout <<"Nome : "<< name <<  endl;
*/

}

void ScanfStmt :: semantics(){

    cout << "Dentro ScanfNode" << endl;
/*    cout <<"Nome : "<< name <<  endl;
*/

}

void BinaryOperator :: semantics(){

    cout << "Dentro BinaryOperator" << endl;
/*    lexpr->semantics();
    rexpr->semantics();*/

}