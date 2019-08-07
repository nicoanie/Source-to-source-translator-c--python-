#include "node.h"
#include "parser.hpp"
#include "translate.h"

/*//Funzione per inserire le funzione dichiarate nella symbol table
Symbol FuncDecl :: semantics(){
    cout << "Dentro FuncDec" << endl;
    cout << type << "," << name << endl;

    string namevar;
    Function func(type, args);

    auto it2 = SymbolTableFunction.find(name);
    //Conto occorrenze della chiave
    int count2 = SymbolTableFunction.count(name);

    //Se la symboltable è vuota inserisce la funzione, altrimenti controlla che non sia già presente una con lo stesso nome
    if (count2 == 0){
        SymbolTableFunction.insert({name, func });
    }
    else{
        //Verifico se la funzione dichiarata è gia presente nella symboltable
        for(int i=0; i<count2; i++){
            if(it2->second.type == type) {
                Error("Function already defined.", line);
            }
            else
                Error("Conflicting type for function.", line);
        }
    }

    //Vado ad aggiornare l'HASH table
    //Trovo tutte le righe con chiave NULL
    auto it = SymbolTableVarAux.find("NULL");
    //Conto occorrenze della chiave
    int count = SymbolTableVarAux.count("NULL");
    for(int i=0; i<count; i++){
        namevar = it->second.scope;
        // Nuovo Symbol e passo nome funzione e tipo della variabile trovata
        it->second.scope=name;
        Symbol y(it->second.scope, it->second.type);
        // Inserisco nella SymbolTable con key->nome della variabile
        SymbolTableVar.insert({namevar, y });
        it++;
    }



   auto it4 = SymbolTableVar.find("FunctionCall");

    //Verifico che i parametri passati siano stati dichiarati TODO
    if (SymbolTableFunctionCall.size()!=0){
        auto it3 = SymbolTableFunctionCall.find("FunctionCall");
        for(int i=0; i<SymbolTableFunctionCall.size(); i++){
            for(int j=0; j<it3->second.par_dich.size(); j++){
                cout << "Nico bello: " << it3->second.par_dich[j]->name<<endl;
            }
        }
    }

        *//*if(SymbolTableVar.count(args[i]->name)==0)
            Error("Use of undeclared identifier.1", line);
        else{
            auto it2 = SymbolTableVar.find(args[i]->name);
            if(it2->second.scope != "global" && it2->second.scope != name)
                Error("Use of undeclared identifier2.", line);
        }*//*
    //TODO: Verifica parametri passati alla chiamata della funzione
*//*    auto it3 = SymbolTableFunctionCall.find("FunctionCall");
    int count3 = SymbolTableVarAux.count("FunctionCall");
    for (int i=0; i< count3; i++){
        for(int j=0; j<it3->second.par_dich.size(); j++){
            if(it3->second.par_dich[j] != //Todo: confrontarli con hash table variabili dichiarate )

        }
    }*//*
    //Cancello contenuti SymbolTableAux
    SymbolTableVarAux.clear();
    SymbolTableFunctionCall.clear();
}



//Funzione per inserire le variabili dichiarate nella symbol table
Symbol VariableDec :: semantics(){
    cout << "Dentro VariableDec" << endl;
    cout << type << "," << name << "Valore : " << void << endl;

    //TODO: Risolvere problema di void

    Symbol var(name, type);

    //Trovo tutte le righe con chiave NULL
    auto it = SymbolTableVarAux.find("NULL");
    //Conto occorrenze della chiave
    int count = SymbolTableVarAux.count("NULL");

    //Se la symboltable è vuota inserisce la variabile, altrimenti controlla che non sia già presente una con lo stesso nome
    if (count == 0){
        SymbolTableVarAux.insert({"NULL", var });
    }
    else{
        //Verifico se la variabile dichiarata è gia presente nella symboltable
        for (int i=0; i<count; i++){
            if (it->second.scope == name)
                // Variabile già dichiarata, segnalo errore
                Error("Variable already defined.", line);
            else
                //Inserisco nella SymbolTable assegnando NULL come chiave in modo tale che dopo quando avrò il nome della funzione
                //posso sostituire tutti i NULL con il nome della variabile e gli scope con il nome della funzione
                SymbolTableVarAux.insert({"NULL", var });
        }
    }
}



Symbol FunctionCall :: semantics(){
    cout <<"Dentro FunctionCall"<<endl;

    CallFunction x (name, args);

    auto it = SymbolTableFunction.find(name);
    int count = SymbolTableFunction.count(name);

    //Se non trovo la funzione deve egnalare errore in quanto non è stata dichiarata
    if (count == 0){
        Error("Implicit declaretion of function.", line);
    }
    else{
        SymbolTableFunctionCall.insert({"FunctionCall", x });
        //Verifico che il numero di parametri della funzione dichiarata siano uguali a quelli che passo
        if(it->second.args.size() > args.size())
            Error("Too few arguments to function call.", line);
        else
            if(it->second.args.size() < args.size())
                Error("Too many arguments to function call.", line);
    }
        //TODO:Verifico che i parametri passati siano stati dichiarati nella funzione o siano globali (va in FuncDecl)

        for(int i=0; i<args.size();i++){
            cout<< "Prova:"<< args[i]->type<<endl;
        }
}

 Symbol ExternalDec :: semantics() {
    //Rendo le variabili dichiarate all'esterno delle funzioni globali
    for(int i=0; i<variables.size(); i++){
        auto it = SymbolTableVar.find(variables[i]->name);
        //cout <<"Prima della modifica" << it-> first << it->second.scope << endl;
        it->second.scope = "global";
        //cout << "Dopo della modifica" << it->second.scope << endl;
    }

    //Verifico che tra le funzioni sia presente una che si chiami main
    if(SymbolTableFunction.count("main") == 0)
        Error("Undefined reference to 'main'.", line);
    else{
        //Verifico che main sia di tipo int o Symbol
        auto it1= SymbolTableFunction.find("main");
        if(it1->second.type != "int" && it1->second.type != "Symbol")
            Error("Wrong function type.", line);
    }

    cout << "Dentro ExternalDec"<<endl;
    //cout << "Variabili presenti:" << variables.size()<<endl;
    //cout << "Funzioni presenti:" << function.size()<<endl;

}

 */

/*--------------------------------------------- TODO: Cose da fare -------------------------------------------------- */


Struttura* ExternalDec :: semantics(){
    cout << "Dentro ExternalDec" << endl;

    //Memorizzo all'interno della symbol table le variabili globali
    for(int i=0; i<variables.size(); i++){
        auto it = SymbolTableVar.find(variables[i]->name);

        if (SymbolTableVar.count(variables[i]->name)==0){
            Symbol var (variables[i]->type, "global");
            SymbolTableVar.insert({variables[i]->name, var});
        }
        else
            Error("Variable already defined.", line);
    }

    //Ispeziono tutte le funzioni che sono state dichiarate
    for(int i=0; i<function.size(); i++){
        function[i]->semantics();

        //cout << "Numero iterazione: " << i << endl;
    }

    //Verifico che tra le funzioni sia presente una che si chiami main
    if(SymbolTableFunction.count("main") == 0)
        Error("Undefined reference to 'main'.", line);
    else{
        //Verifico che main sia di tipo int o Symbol
        auto it1= SymbolTableFunction.find("main");
        if(it1->second.type != "int" && it1->second.type != "void")
            Error("Wrong function type.", line);
    }
}

Struttura* FuncDecl :: semantics(){
    cout << "Dentro FuncDecl" << endl;

    Struttura* v;

    //cout << "Nome Funzione: " << name << endl;
    Function func (type, args);

    //Verifico che la funzione non sia già presente all'interno della Symbol table e nel caso la aggiungo
    if(SymbolTableFunction.count(name)==0){
        SymbolTableFunction.insert({name, func});
    }
    else{
        auto it = SymbolTableFunction.find(name);
        if(it->second.type == type)
            Error("Function already defined.", line);
        else
            Error("Conflicting type for function.", line);
    }

    //Ispeziono il corpo della funzione
    for(int i=0; i<body.size(); i++){
        //body[i]->semantics();

        v = body[i]->semantics();
        auto control = v->controllo;

        if(control == "variable"){
            //Verifico le variabili dichiarate
            auto x = v->type;
            auto y = v->name;
            auto z = v->linea;
            //TODO: Risolvere problema del numero della linea
            Symbol var (x, name);
            auto it = SymbolTableVar.find(y);
            for(int i=0; i<SymbolTableVar.count(y);i++){
                if(it->second.scope==name)
                    Error("Variable already defined.", z);
            }
            SymbolTableVar.insert({y, var});
        }

        if(control == "functioncall")
            cout << "TODO: Effettuare le verifiche" << endl;
    }
}

Struttura* VariableDec :: semantics(){

    cout << "Dentro VariableDecNode" << endl;

    //Verifico se è già presente una variabile con lo stesso nome
    if(SymbolTableVar.count(name)!=0){
        //Verifico che non sia una variabile globale
        auto it = SymbolTableVar.find(name);
        for(int i=0; i< SymbolTableVar.count(name); i++){
            if(it->second.scope == "global" )
                Error("Variable already defined.", line);
        }
    }

    //Devo verificare che non sia già presente nella symbol table
    Struttura s(name,type,line,"variable",0);
    Struttura* v (&s);
    return v;
}


Struttura* FunctionCall :: semantics(){

    cout << "Dentro FunctionCall" << endl;

    CallFunction x (name, args);

    auto it = SymbolTableFunction.find(name);
    int count = SymbolTableFunction.count(name);

    //Se non trovo la funzione deve egnalare errore in quanto non è stata dichiarata
    if (count == 0){
        Error("Implicit declaretion of function.", line);
    }
    else{
        SymbolTableFunctionCall.insert({"FunctionCall", x });
        //Verifico che il numero di parametri della funzione dichiarata siano uguali a quelli che passo
        if(it->second.args.size() > args.size())
            Error("Too few arguments to function call.", line);
        else
        if(it->second.args.size() < args.size())
            Error("Too many arguments to function call.", line);
    }

    //TODO:Verifico che i parametri passati siano stati dichiarati nella funzione o siano globali e corrispondano al giusto tipo
    Struttura s(name,"brodo",line,"functioncall",0);// TODO: Devo restituire args e non queste cose, le ho messe giusto per vedere se avessi risolto un errore... continuo domani il sonno si fa sentire
    Struttura* v (&s);
    return v;
}

Struttura* IntegerNode :: semantics(){
    cout << "Dentro IntegerNode" << endl;
    /*   cout <<"Nome : "<< name << "Valore : " << void << endl;*/

}

Struttura* FloatNode :: semantics(){
    cout << "Dentro FloatNode" << endl;
    /*   cout <<"Nome : "<< name << "Valore : " << void << endl;*/

}

Struttura* CharNode :: semantics(){
    cout << "Dentro CharNode" << endl;
    /*cout <<"Nome : "<< name << "Valore : " << void << endl;*/

}

Struttura* BoolNode :: semantics(){
    cout << "Dentro BoolNode" << endl;
    /*cout <<"Nome : "<< name << "Valore : " << void << endl;*/

}

Struttura* IdentifierNode :: semantics(){
   cout << "Dentro IdentifierNode" << endl;
    /* cout <<"Nome : "<< name << "Valore : " << void << endl;*/
}

Struttura* Assignment :: semantics(){

    cout << "Dentro AssignmentNode" << endl;
/*    cout <<"Nome : "<< name << "Valore : " << void << endl;
*/

}

Struttura* IfStmt :: semantics(){

    cout << "Dentro IfNode" << endl;
/*    cout <<"Nome : "<< name <<  endl;
*/

}

Struttura* WhileStmt :: semantics(){

    cout << "Dentro WhileNode" << endl;
/*    cout <<"Nome : "<< name <<  endl;
*/

}

Struttura* ReturnStmt :: semantics(){

    cout << "Dentro ReturnNode" << endl;
/*    cout <<"Nome : "<< name <<  endl;
*/

}

Struttura* PrintStmt :: semantics(){

    cout << "Dentro ReturnNode" << endl;
/*    cout <<"Nome : "<< name <<  endl;
*/

}

Struttura* ScanfStmt :: semantics(){

    cout << "Dentro ReturnNode" << endl;
/*    cout <<"Nome : "<< name <<  endl;
*/

}

Struttura* BinaryOperator :: semantics(){

    cout << "Dentro ReturnNode" << endl;
/*    cout <<"Nome : "<< name <<  endl;
*/

}