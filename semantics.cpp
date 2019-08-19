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
        if(it1->second.args.size() != 0)
            Error("Wrong parameter declaration.", line);
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

    //Se la funzione viene trovata verifico che non ci sia una variabile con lo stesso nome in function declaration
    if(SymbolTableVar.count(name) != 0){
        auto it = SymbolTableVar.find(name);
        for(int i=0; i<SymbolTableVar.count(name); i++){
            if(it->second.scope == name_function_declaration || it->second.scope == "global")
                Error("called object is not a function or function pointer", line);
        }
    }

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
        f_c = 1;
        //Richiamo il nodo identifier per ogni parametro ed effettuo i controlli
        args[i]->semantics();
        f_c = 0;
    }

    //Variabile utile a segnalare in che nodo sono entrato
    nodo = 6;
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
            //Controllo se è stato il nodo FunctionCall a richiamare
            //Se è vero, vado a controllare se coincidie anche il tipo della variabile
            if(f_c==1){
                if(it->second.type != it2->second.args[iterazione]->type)
                    Error("Error type variable.", line);
                //TODO: Gestire casting varibili
                found = true; // Corrispondenza variabile trovata
                i=SymbolTableVar.count(value);
            }
            //Se è falso, segnalo che la variabile è stata trovata
            else{
                found = true; // Corrispondenza variabile trovata
                //Memorizzo di che tipo è la varibile
                if(it->second.type == "int") tipo=1;
                if(it->second.type == "float") tipo=2;
                if(it->second.type == "char") tipo=3;
                if(it->second.type == "bool") tipo=4;

                i=SymbolTableVar.count(value);
            }
        }
        it++;
    }

    if (!found)
        Error("Variable undeclared (first use in this function).", line);

    //Variabile utile per le verifiche nel nodo return
    nodo = 3;

}

void Assignment :: semantics(){

    cout << "Dentro AssignmentNode" << endl;
    //Verifico che la variabile sia stata dichiarata
    bool found;
    string tipovar;

    auto it = SymbolTableVar.find(name);
    for(int i=0; i<SymbolTableVar.count(name);i++){
        if(it->second.scope == name_function_declaration || it->second.scope == "global"){
            found = true;
            tipovar = it->second.type;
            i=SymbolTableVar.count(name);
        }
        it++;
    }
    if (!found)
        Error("Variable undeclared.", line);

    value->semantics();

    auto it2 = SymbolTableFunction.find(name_function_call);
    //Gestisco il casting
    if(nodo == 6){
        if(tipovar == "int" && it2->second.type == "float" ) Warning("Narrowing conversion from 'float' to 'int'.", line);
        if(tipovar == "char" && it2->second.type == "float" ) Warning("Narrowing conversion from 'float' to 'char'.", line);
        if(tipovar == "bool" && it2->second.type == "float" ) Warning("Narrowing conversion from 'float' to 'bool'.", line);
    }
}

void IntegerNode :: semantics(){
    cout << "Dentro IntegerNode" << endl;
    value_type_node = value;

    //Variabile utile per le verifiche nel nodo return
    nodo = 1;
}

void FloatNode :: semantics(){
    cout << "Dentro FloatNode" << endl;
    value_type_node = value;

    //Variabile utile per le verifiche nel nodo return
    nodo = 2;
}

void CharNode :: semantics(){
    cout << "Dentro CharNode" << endl;
    value_type_node = value;

    //Variabile utile per le verifiche nel nodo return
    nodo = 4;
}

void BoolNode :: semantics(){
    cout << "Dentro BoolNode" << endl;
    if (value == "true") value_type_node = 1;
    if (value == "false") value_type_node = 0;

    //Variabile utile per le verifiche nel nodo return
    nodo = 5;
}


void IfStmt :: semantics(){

    cout << "Dentro IfNode" << endl;

    //TODO: Visto che non ho stringhe, dovrei solo capire bene l'overfitting delle variabili
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

    //TODO: Come per la condizione dell'if, non avendo le stringhe, la condizione dovrebbe essere sempre possibile portarla al formato booleano
    condition->semantics();
    for(int i=0; i<body_while.size(); i++){
        cout << "Body While" << endl;
        body_while[i]->semantics();
    }

}

void ReturnNode :: semantics(){

    cout << "Dentro ReturnNode" << endl;
    value->semantics();

    auto it = SymbolTableFunction.find(name_function_declaration);

    //In base al tipo di funzione, stabilisco cosa è possibile restituire

    //Restrizioni per funzioni di tipo intero
    if(it->second.type == "int"){
        //Se si è entrati nel nodo identifier, bisogna vedere di che tipo è la variabile restituita
        if(nodo==3){
            if(tipo == 2) Warning("Narrowing conversion from 'float' to 'int'.", line);
        }
        if(nodo==2) Warning("Narrowing conversion from 'float' to 'int'.", line);
    }

    //Restrizioni per funzioni di tipo char
    if(it->second.type == "char"){
        //Se si è entrati nel nodo identifier, bisogna vedere di che tipo è la variabile restituita
        if(nodo==3){
            if(tipo == 2) Warning("Narrowing conversion from 'float' to 'char'.", line);
        }
        if(nodo==2) Warning("Narrowing conversion from 'float' to 'char'.", line);
    }

    //Restrizioni per funzioni di tipo bool
    if(it->second.type == "bool"){
        //Se si è entrati nel nodo identifier, bisogna vedere di che tipo è la variabile restituita
        if(nodo==3){
            if(tipo == 2) Warning("Narrowing conversion from 'float' to 'bool'.", line);
        }
        if(nodo==2) Warning("Narrowing conversion from 'float' to 'bool'.", line);
    }
}


/*--------------------------------------------- TODO: Cose da fare -------------------------------------------------- */

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

    lexpr->semantics();
    lexpr_value = value_type_node;
    rexpr->semantics();
    rexpr_value = value_type_node;

    switch(op){
        case T_PLUS	: cout << "+" << endl;
            break;
        case T_MINUS	: cout << "-" << endl;
            break;
        case T_MULTIPLY	: cout << "*" << endl;
            break;
        case T_DIVIDE	: if(rexpr_value == 0)  Warning("Division by zero", line);
            break;
        case T_LT	: cout << "<"<< endl;
            break;
        case T_GT	: cout << ">" << endl;
            break;
        case T_GE	: cout << ">=" << endl;
            break;
        case T_LE	: cout << "<=" << endl;
            break;
        case T_AND	: cout << "&&" << endl;
            break;
        case T_OR	: cout << "or" << endl;
            break;
        case T_NOT	: cout << "not" << endl;
            break;
        case T_EQUAL	: cout << "=" << endl;
            break;
        case T_NOTEQUAL	: cout << "!=" << endl;
            break;
        default		: Error("Unavalaible expression.", line);
    }



/*    if(op == T_DIVIDE){
        if(rexpr_value == 0)  Warning("Division by zero", line);
    }*/

/*    int idl = 0; //Per controllare espressione a sinistra
    int idr = 0; //Per controllare espressione a destra

    IDENT = 0; //Setto a 0 e vado a vedere se dopo lexpr cambia
    lexpr->semantics();
    idl = IDENT;
    IDENT = 0;
    rexpr->semantics();
    idr = IDENT;

    if (idl == 0 || idr == 0)	Error("Missing operand.", line);*/

}