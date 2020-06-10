#include "node.cpp"
#include "parser.hpp"
#include "semantics.h"
#include "translate.h"


void IntegerNode :: semantics(){
    value_type_node = value;

    //Variabile utile per le verifiche nel nodo return
    nodo = 1;
}

void FloatNode :: semantics(){
    value_type_node = value;

    //Variabile utile per le verifiche nel nodo return
    nodo = 2;
}

void CharNode :: semantics(){
    value_type_node = value;

    //Variabile utile per le verifiche nel nodo return
    nodo = 4;
}

void BoolNode :: semantics(){
    if (value == "true") value_type_node = 1;
    if (value == "false") value_type_node = 0;

    //Variabile utile per le verifiche nel nodo return
    nodo = 5;
}

void StringNode::semantics() {

}

void ExternalDec :: semantics(){
    bool found = false;
    int countvett = 0;

    for(int i=0; i<librerie.size(); i++){
        librerie[i]->semantics();
        for(int j=0; j<2; j++){
            if(librerie[i]->name == Library[j]){
                found = true;
                j=2;
            }
        }
        if(!found){
            Library[countvett] = librerie[i]->name;
            countvett++;
        }
        found = false;
    }

    //Memorizzo all'interno della symbol table le variabili globali
    for(int i=0; i<variables.size(); i++){
        //Memorizzo nella variabile it le informazioni ricercate nella SymbolTableVar
        auto it = SymbolTableVar.find(variables[i]->name);

        //Se non trovo niente, significa che posso memorizzare la variabile
        if (SymbolTableVar.count(variables[i]->name)==0){

            variables[i]->semantics();

            //Creo una nuova variabile di tipo Symbol contenente il tipo della variabile e la funzione di appartenenza
            Symbol var (variables[i]->type, "global");
            //La memorizzo in SymbolTableVar
            SymbolTableVar.insert({variables[i]->name, var});
        }
        else
            Error("Variable already defined.", line);
    }

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

void LibDec ::semantics() {
    if( name != "stdio" && name != "stdbool") Error("E' possibile utilizzare solo le librerie stdio.h e bool.h .", line);
}

void FuncDecl :: semantics(){
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

    if(return_in == 0 && name != "main"){
        Error("Return not found in '" + name + "' function.", line);
    }
    else{
        return_in = 0;
    }
}

void VariableDec :: semantics(){

    if(type == "bool"){
        bool found2 = false;

        for(int i=0; i<2; i++){
            if(Library[i] == "stdbool") found2 = true;
        }

        if(!found2) Error("Unknown type name ‘bool’, include <stdbool.h> ", line);
    }

    //Verifico se è già presente una variabile con lo stesso nome
    if(SymbolTableVar.count(name)!=0){
        //Verifico che non sia una variabile globale e non sia già stata assegnata a quella funzione
        auto it = SymbolTableVar.find(name);
        for(int i=0; i< SymbolTableVar.count(name); i++){
            if(it->second.scope == name_function_declaration && it->second.type != type )
                Error("Conflicting type for function.", line);
            if(it->second.scope == name_function_declaration && it->second.type == type )
                Error("Variable already defined.", line);
            it++;
        }
    }

    if(value) {
        value->semantics();

        Symbol var(type, name_function_declaration);
        //Inserisco nella SymbolTable la variabile
        SymbolTableVar.insert({name, var});
    }
    else{
        Symbol var(type, name_function_declaration);
        //Inserisco nella SymbolTable la variabile
        SymbolTableVar.insert({name, var});
    }
}


void FunctionCall :: semantics(){

    aux_tipo = tipo; //Tengo traccia del tipo di variabile che ha chiamato FunctionCall

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

    //Verifico che la variabile è stata dichiarata all'interno della funzione
    if(SymbolTableVar.count(value) == 0)
        Error("Variable undeclared (first use in this function).", line);

    //Se è presente nella symbol table devo verificare che appartenga alla funzione o sia globale
    bool found;
    auto it = SymbolTableVar.find(value);

    //Versione controllo dei tipi
    for(int i=0; i< SymbolTableVar.count(value); i++){

        if(it->second.scope == name_function_declaration || it->second.scope == "global"){
            //Controllo se è stato il nodo FunctionCall a richiamare
            //Se è vero, vado a controllare se coincidie anche il tipo della variabile

            if(f_c==1){

                auto it2= SymbolTableFunction.find(name_function_call);
                if((it->second.type == "float") && (it2->second.args[iterazione]->type == "int" || it2->second.args[iterazione]->type == "char" || it2->second.args[iterazione]->type == "bool")) //it contiene variabili trovate nella symbol table, it2 contiene le funzioni dichiarate
                    Warning("Narrowing conversion from 'float' to other type.", line);
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

void IfStmt :: semantics(){

    condition->semantics();
    for(int i=0; i<body_if.size();i++){
        body_if[i]->semantics();
    }
    for(int i=0; i<body_else.size(); i++){
        body_else[i]->semantics();
    }
}

void WhileStmt :: semantics(){

    nodo = 8;
    condition->semantics();
    for(int i=0; i<body_while.size(); i++){
        body_while[i]->semantics();
    }

}

void ReturnNode :: semantics(){

    return_in = 1;
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

void PrintStmt :: semantics(){
    if(!string_v){
        string aux2;
        bool found2 = false;

        for(int i=0; i<2; i++){
            if(Library[i] == "stdio") found2 = true;
        }
        //Verifico che sia presente la libreria stdio.h
        if(!found2) Error("Implicit declaration of function ‘printf’, include <stdio.h> ", line);

        //Verifico che lo specifier inserito sia uno valido
        if(specifiers != "\"%d\"" && specifiers != "\"%f\"" && specifiers != "\"%c\"") Error("Invalid conversion specifier.", line);

        //Verifico che la variabile sia stata dichiarata
        auto it = SymbolTableVar.find(name);
        bool found;
        for(int i=0; i<SymbolTableVar.count(name); i++){
            if(it->second.scope == name_function_declaration || it->second.scope== "global"){
                if(it->second.scope == name_function_declaration){
                    found= true;
                    aux2 = it->second.type;
                    i = SymbolTableVar.count(name);
                }
                else{
                    found= true;
                    aux2 = it->second.type;
                }
            }
            it++;
        }
        //Se la variabile non è stata trovata segnalo l'errore

        if(!found) Error("Variable undeclared (first use in this function).", line);
        else{
            //Se la variabile è stata trovata confronto il tipo della variabile con lo specifier inserito
            if(specifiers == "\"%d\"" && aux2 == "float") Error("Format specifies types 'int' but the arguments has type 'float'", line);
            if(specifiers == "\"%f\"" && aux2 != "float") Error("Format specifies types 'float' but the arguments has other type ", line);
            if(specifiers == "\"%c\"" && aux2 == "float") Error("Format specifies types 'int' but the arguments has type 'float'", line);
        }
    }
    else{
        string_v->semantics();
    }

}

void ScanfStmt :: semantics(){

    bool found2 = false;

    for(int i=0; i<2; i++){
        if(Library[i] == "stdio") found2 = true;
    }

    if(!found2) Error("Implicit declaration of function ‘scanf’, include <stdio.h> ", line);

    if(specifiers != "\"%d\"" && specifiers != "\"%f\"" && specifiers != "\"%c\"") Error("Invalid conversion specifier.", line);

    //Verifico che la variabile sia stata dichiarata
    auto it = SymbolTableVar.find(name);
    bool found;

    for(int i=0; i<SymbolTableVar.count(name); i++){
        if(it->second.scope == name_function_declaration || it->second.scope== "global"){
            found= true;
            i= SymbolTableVar.count(name);
        }
    }

    //Se la variabile non è stata trovata segnalo l'errore
    if(!found) Error("Variable undeclared (first use in this function).", line);
    else{
        //Se la variabile è stata trovata confronto il tipo della variabile con lo specifier inserito
        if(specifiers == "\"%d\"" && it->second.type != "int") Error("Format specifies types 'int*' but the arguments has other type. ", line);
        if(specifiers == "\"%f\"" && it->second.type != "float") Error("Format specifies types 'float*' but the arguments has other type. ", line);
        if(specifiers == "\"%c\"" && it->second.type != "char") Error("Format specifies types 'char*' but the arguments has other type. ", line);
    }
}

void BinaryOperator :: semantics(){

    bool error;

    if(!lexpr){
        rexpr->semantics();
        R = Type_Expr(nodo, tipo, aux_tipo);

        L = "bool";

        if((R == "int" || R == "float" || R == "char" || R == "bool")){
            switch(op){
                case T_NOT	:
                    break;

                default		: Error("Unavalaible expression.", line);
            }

            error = false; //Setto che non deve dare errore in quanto ha trovato un operatore da utilizzare
        }

    }

    else{
        lexpr->semantics();
        L = Type_Expr(nodo, tipo, aux_tipo);
        rexpr->semantics();
        rexpr_value = value_type_node;
        R = Type_Expr(nodo, tipo, aux_tipo);

        if((L == "int"|| L == "float" || L == "char" || L == "bool") && (R == "int" || R == "float" || R == "char" || R == "bool")){

            switch(op){
                case T_PLUS	:
                    break;
                case T_MINUS	:
                    break;
                case T_MULTIPLY	:
                    break;
                case T_DIVIDE	:
                    //Verifico che non sia entrato in identifier, perchè in quel caso non avrei il valore della variabile
                    //Posso utilizzare nodo perchè controllando per secondo il valore dell'espressione a destra, in nodo resta memorizzato quello che mi serve
                    if(nodo != 3 && rexpr_value == 0)  Warning("Division by zero", line);
                    break;
                case T_LT	:
                    break;
                case T_GT	:
                    break;
                case T_GE	:
                    break;
                case T_LE	:
                    break;
                case T_EQUAL	:
                    break;
                case T_NOTEQUAL	:
                    break;
                case T_AND	:
                    break;
                case T_OR	:
                    break;

                default		: Error("Unavalaible expression.", line);
            }

            error = false; //Setto che non deve dare errore in quanto ha trovato un operatore da utilizzare

        }
    }
    if(error)     Error("Unrecognised type of operand.", line);
}

/*--------------------------------------------------- TRADUZIONE --------------------------------------------------- */

void IntegerNode :: translate(){

    if((nodo == 8 && while_condition == 1) || (nodo == 10 && if_condition == 1)){
        ass_whil_if_value = to_string(value);
        if(value == 0){
            trad.open("python.py", ios::app);
            trad << "False";
            trad.close();
        }
        else{
            trad.open("python.py", ios::app);
            trad << "True";
            trad.close();
        }
    }
    else{
        trad.open("python.py", ios::app);
        trad << value;
        trad.close();

        nodo=1;
    }

}

void FloatNode :: translate(){

    if((nodo == 8 && while_condition == 1) || (nodo == 10 && if_condition == 1)){
        ass_whil_if_value = to_string(value);
        if(value == 0){
            trad.open("python.py", ios::app);
            trad << "False";
            trad.close();
        }
        else{
            trad.open("python.py", ios::app);
            trad << "True";
            trad.close();
        }
    }
    else{
        trad.open("python.py", ios::app);
        trad << value;
        trad.close();

        nodo=2;
    }
}

void CharNode :: translate(){

    if((nodo == 8 && while_condition == 1) || (nodo == 10 && if_condition == 1)){
        string aux_string = "'";
        ass_whil_if_value = aux_string +value+ aux_string;
        trad.open("python.py", ios::app);
        trad << "True";
        trad.close();
    }
    else{
        trad.open("python.py", ios::app);
        trad << "ord('" << value << "')";
        trad.close();
        nodo = 4;
    }
}

void BoolNode :: translate(){

    if((nodo == 8 && while_condition == 1) || (nodo == 10 && if_condition == 1)){
        if (value == "true"){
            ass_whil_if_value = "True";
            trad.open("python.py", ios::app);
            trad << "True";
            trad.close();
        }
        if (value == "false"){
            ass_whil_if_value = "False";
            trad.open("python.py", ios::app);
            trad << "False";
            trad.close();
        }
    }
    else{
        trad.open("python.py", ios::app);

        if (value == "true") trad << "True";
        if (value == "false") trad << "False";

        trad.close();

        nodo = 5;
    }
}

void StringNode::translate() {
    trad.open("python.py", ios::app);
    trad << "print("<<value<< ")";
    trad.close();
}

void ExternalDec :: translate(){
    trad.open("python.py");
    trad.close();
    // Traduco in python tutte le variabili a cui viene assegnato un valore
    // Quelle solo dichiarate non vengono tradotte in quanto non è possibile in python
    for (int i=0; i<variables.size(); i++){
        global_var.push_back(variables[i]);
        variables[i]->translate();
        trad.open("python.py", ios::app);
        trad << endl;
        trad.close();
    }

    // Traduco tutte le funzioni dichiarate
    for(int i=0; i<function.size(); i++){
        function[i]->translate();
    }

}

void LibDec :: translate() {

}

void FuncDecl :: translate(){

    name_function_declaration = name;

    trad.open("python.py", ios::app);
    trad << "def "<< name<<"( ";

    // Se la funzione non ha argomenti
    if (args.size() == 0)
        trad << "):" << endl;

    // Scrivo gli argomenti richiesti dalla funzione nel caso sono presenti
    for(int i=0; i<args.size(); i++){
        if(i == args.size()-1)
            trad << args[i]->name << "):" << endl;
        else
            trad << args[i]->name << "," ;
    }

    trad.close();

    //Dichiaro le variabili globali nel caso siano presenti
    bool found = false;
    //Per ogni variabile globale presente nel vettore
    for(int i =0; i<global_var.size(); i++){
        auto it = SymbolTableVar.find(global_var[i]->name);
        //Vado a verificare se sono presenti all'interno del vettore olre che con scope = global anche con scope = name_function_declaration
        for(int j=0; j<SymbolTableVar.count(global_var[i]->name);j++){
            //Se ne trovo una con scope=nome funzione che sto gestendo esco dal ciclo for
            if(it->second.scope == name_function_declaration){
                found = true;
            }
            it++;
        }
        //Se una variabile con scope = nome funzione che sto gestendo non è stata trovata la dichiaro con la keyword in python
        if(found != true){
            indentazione ++;
            indent(indentazione);
            trad.open("python.py", ios::app);
            trad << "global " << global_var[i]->name<<endl;
            trad.close();
            indentazione --;
        }
        found = false;
    }

    // Traduco il corpo della funzione
    for(int i=0; i<body.size(); i++){
        indentazione ++;
        indent(indentazione);
        body[i]->translate();
        trad.open("python.py", ios::app);
        trad << endl;
        trad.close();
        indentazione --;
    }

}

void VariableDec :: translate(){

    // Se oltre ad essere dichiarata è stato assegnato un valore alla variabile la scrivo sul file
    if(valore != "NULL"){
        trad.open("python.py", ios::app);

        // Scrivo sul file l'intestazione della funzione
        trad << name << " = ";

        trad.close();

        //Devo considerare solo quelle a cui è stato assegnato un valore
        value->translate();
    }

        //Altrimenti setto i valori uguali a 0
    else{
        // Mi serve dichiarare comunque la variabile altrimenti non posso utilizzarla, nel caso, come parametro quando richiamo una funzione
        if(type == "int" || type == "float" || type == "char"){
            trad.open("python.py", ios::app);
            trad << name << " = 0 ";
            trad.close();
        }

        if(type == "bool"){
            trad.open("python.py", ios::app);
            trad << name << " = False ";
            trad.close();
        }

    }


}

void Assignment :: translate(){
    assignment_in = 1;

    if((nodo == 8 && while_condition == 1) || (nodo == 10 && if_condition == 1)){
        ass_whil_if_name = name;
        value->translate();
    }
    else{

        name_variable = name;
        nodo = 7;

        trad.open("python.py", ios::app);
        trad << name << " = ";
        trad.close();

        auto it= SymbolTableVar.find(name_variable);

        for(int i=0; i<SymbolTableVar.count(name_variable); i++){
            if(it->second.scope == name_function_declaration || it->second.scope == "global"){
                if(it->second.scope == name_function_declaration){
                    if(it->second.type == "int") tipo=1;
                    if(it->second.type == "float") tipo=2;
                    if(it->second.type == "char") tipo=3;
                    if(it->second.type == "bool") tipo=4;
                    i = SymbolTableVar.count(name_variable);
                }
                else{
                    if(it->second.type == "int") tipo=1;
                    if(it->second.type == "float") tipo=2;
                    if(it->second.type == "char") tipo=3;
                    if(it->second.type == "bool") tipo=4;
                }
            }
            it++;
        }

        value->translate();
    }
    assignment_in = 0;

}

void FunctionCall :: translate(){

    int par; // Per vedere quante parentesi devo mettere alla fine

    if (nodo == 7){
        // Ricavo il tipo della variabile
        auto it = SymbolTableVar.find(name_variable);

        for(int i =0; i<SymbolTableVar.count(name_variable); i++){
            if(it->second.scope == "global" || it->second.scope == name_function_declaration){
                if(it->second.scope == name_function_declaration){
                    type_variabile = it->second.type;
                    i=SymbolTableVar.count(name_variable);
                }
                else{
                    type_variabile = it->second.type;
                }

            }
            it++;
        }

        // Ricavo il tipo della funzione
        auto it2 = SymbolTableFunction.find(name);

        // Caso variabile intera e char
        if (type_variabile == "int" || type_variabile == "char") {
            if (it2->second.type == "int" || it2->second.type == "char") {
                trad.open("python.py", ios::app);
                trad << name << "( ";
                par = 1;
            }
            if (it2->second.type == "float") {
                trad.open("python.py", ios::app);
                trad << "int( " << name << "( ";
                par = 2;
            }
            if (it2->second.type == "bool") {
                trad.open("python.py", ios::app);
                trad << "int( " << name << "( ";
                par = 2;
            }
        }
        // Caso variabile float
            if (type_variabile == "float") {
                if (it2->second.type == "int" || it2->second.type == "char") {
                    trad.open("python.py", ios::app);
                    trad << name << "( ";
                    par = 1;
                }
                if (it2->second.type == "float") {
                    trad.open("python.py", ios::app);
                    trad << name << "( ";
                    par = 1;
                }
                if (it2->second.type == "bool") {
                    trad.open("python.py", ios::app);
                    trad << "float( " << name << "( ";
                    par = 2;
                }
            }
        // Caso variabile bool
        if (type_variabile == "bool"){
            if(it2->second.type == "int" || it2->second.type == "char") {
                trad.open("python.py", ios::app);
                trad << "bool( " << name << "( ";
                par=2;
            }
            if(it2->second.type == "float") {
                trad.open("python.py", ios::app);
                trad << " bool( " << name << "( ";
                par=2;
            }

            if(it2->second.type == "bool") {
                trad.open("python.py", ios::app);
                trad << name << "( ";
                par=1;
            }
        }

        // Se non ci sono argomenti
        if (args.size() == 0){
            for(int i=0; i<par; i++){
                trad << ")" ;
            }
            trad.close();
        }
        else{
            trad.close();
            // Scrivo i parametri da passare nel caso sono presenti
            for(int i=0; i<args.size(); i++){
                if(i == args.size()-1){
                    args[i]->translate();
                    trad.open("python.py", ios::app);
                    for(int j=0; j<par; j++){
                        trad << " )";
                    }
                    trad.close();
                }
                else{
                    args[i]->translate();
                    trad.open("python.py", ios::app);
                    trad << ", " ;
                    trad.close();
                }
            }
        }
    } // Fine controlli nodo Assignment

    else{
        trad.open("python.py", ios::app);
        trad << name << "( ";
        if (args.size() == 0){
            trad << ")";
            trad.close();
        }
        else{
            trad.close();
            // Scrivo i parametri da passare nel caso sono presenti
            for(int i=0; i<args.size(); i++){
                iterazione = i; //Memorizzo il numero dell'iterazione
                f_c = 1;
                if(i == args.size()-1){
                    args[i]->translate();
                    trad.open("python.py", ios::app);
                    trad << " )";
                    trad.close();
                }
                else{
                    args[i]->translate();
                    trad.open("python.py", ios::app);
                    trad << ", " ;
                    trad.close();
                }
                f_c = 0;
            }
        }

    }

    nodo =6;
}

void IdentifierNode :: translate(){

    auto it= SymbolTableVar.find(value);
    bool in = false;

    for(int i=0; i<SymbolTableVar.count(value); i++){

        if(it->second.scope == name_function_declaration || it->second.scope == "global"){
            if(it->second.scope == name_function_declaration){
                //value_type_node = it->second.val;
                if(it->second.type == "int") tipo=1;
                if(it->second.type == "float") tipo=2;
                if(it->second.type == "char") tipo=3;
                if(it->second.type == "bool") tipo=4;
                i=SymbolTableVar.count(value);
            }
            else{
                //value_type_node = it->second.val;
                if(it->second.type == "int") tipo=1;
                if(it->second.type == "float") tipo=2;
                if(it->second.type == "char") tipo=3;
                if(it->second.type == "bool") tipo=4;
            }
        }
        it++;
    }

    //Per gestire assegnazione nella condizione while e if nel caso in cui viene assegnata un'espressione matematica
    if(assignment_in==1 && nodo == 9 && utile && name_variable == ""){
        trad.open("python.py", ios::app);
        trad << value;
        trad.close();
    }

    //Per gestire assegnazione nella condizione while e if nel caso in cui viene assegnata una variabile
    if((nodo == 8 && while_condition == 1) || (nodo == 10 && if_condition == 1)){
      ass_whil_if_value = value;

      if(assignment_in ==1){
          if(new_file){
              new_file = false;
              trad.open("aux.py", ios::app);
              trad << "def struct(x):"<< endl;
              trad << "\t if(x == 0):"<< endl;
              trad << "\t \t return False" << endl;
              trad << "\t else:" << endl;
              trad << "\t \t return True"<< endl;
              trad.close();
          }

          if(Bin_Op == 1){
              trad.open("python.py", ios::app);
              trad << "struct("<< value;
              trad.close();
          }
          else{
              in = true;
              trad.open("python.py", ios::app);
              trad << "struct("<< value << ")";
              trad.close();
          }
        }
      else{
          in = true;
          if(value_type_node == 0 && tipo != 3){
              trad.open("python.py", ios::app);
              trad << "True";
              trad.close();
          }
          else{
              trad.open("python.py", ios::app);
              trad << "False";
              trad.close();
          }
      }
    }

    //Casting parametri delle funzioni
    if(f_c == 1){
        auto it2= SymbolTableFunction.find(name_function_call);
        // Controlli parametro int
        if((it2->second.args[iterazione]->type == "int" || it2->second.args[iterazione]->type == "char") && (tipo == 1 || tipo == 3)){
            trad.open("python.py", ios::app);
            trad << value;
            trad.close();
        }
        if(it2->second.args[iterazione]->type == "int" && tipo == 2 ){
            trad.open("python.py", ios::app);
            trad << "int( " << value << " )";
            trad.close();
        }
        if(it2->second.args[iterazione]->type == "int" && tipo == 4){
            trad.open("python.py", ios::app);
            trad << "int( " << value << " )";
            trad.close();
        }

        // Controlli parametro float
        if(it2->second.args[iterazione]->type == "float" && (tipo == 1 || tipo == 2 || tipo == 3)){
            trad.open("python.py", ios::app);
            trad << value;
            trad.close();
        }

        if(it2->second.args[iterazione]->type == "float" && tipo == 4 ){
            trad.open("python.py", ios::app);
            trad << "float( " << value << " )";
            trad.close();
        }

        // Controlli parametro bool
        if(it2->second.args[iterazione]->type == "bool" && tipo == 4){
            trad.open("python.py", ios::app);
            trad << value;
            trad.close();
        }
        if(it2->second.args[iterazione]->type == "bool" && (tipo == 1 || tipo == 2 || tipo == 3)){
            trad.open("python.py", ios::app);
            trad << "bool( " << value << " )";
            trad.close();
        }
    }

    else{
        if(assignment_in==1 && nodo == 9){
            auto it2= SymbolTableVar.find(name_variable);
            for(int i=0; i<SymbolTableVar.count(name_variable); i++){
                if(it2->second.scope == name_function_declaration){
                    aux_typ = it2->second.type;
                    i = SymbolTableVar.count(name_variable);
                }
                else {
                    aux_typ = it2->second.type;
                }
                it2++;
            }
            // Controlli parametro int
            if(aux_typ == "int" && (tipo == 1 || tipo == 3)){
                trad.open("python.py", ios::app);
                trad << value;
                trad.close();
            }
            if(aux_typ == "int" && tipo == 2 ){
                trad.open("python.py", ios::app);
                trad << value;
                trad.close();
            }
            if(aux_typ == "int" && tipo == 4) {
                trad.open("python.py", ios::app);
                trad << "int( " << value << " )";
                trad.close();
            }

            // Controlli parametro float
            if(aux_typ == "float" && (tipo == 1 || tipo == 2)){
                trad.open("python.py", ios::app);
                trad << value;
                trad.close();
            }
            if(aux_typ == "float" && tipo == 3){
                trad.open("python.py", ios::app);
                trad << "ord( " << value << " )";
                trad.close();
            }
            if(aux_typ == "float" && tipo == 4 ){
                trad.open("python.py", ios::app);
                trad << "float( " << value << " )";
                trad.close();
            }

            // Controlli parametro char
            if(aux_typ == "char" && (tipo == 1 || tipo == 3)){
                trad.open("python.py", ios::app);
                trad << value;
                trad.close();
            }
            if(aux_typ == "char" && tipo == 2 ){
                trad.open("python.py", ios::app);
                trad << value;
                trad.close();
            }
            if(aux_typ == "char" && tipo == 4) {
                trad.open("python.py", ios::app);
                trad << "int( " << value << " )";
                trad.close();
            }

            // Controlli parametro bool
            if(aux_typ == "bool" && tipo == 4){
                trad.open("python.py", ios::app);
                trad << "bool( " << value << " )";
                trad.close();
            }
            if(aux_typ == "bool" && (tipo == 1 || tipo == 2 || tipo == 3)){
                trad.open("python.py", ios::app);
                trad << "bool( " << value << " )";
                trad.close();
            }
        }
        else{
            if(assignment_in==1 && nodo != 9 && in != 1){

                auto it2= SymbolTableVar.find(name_variable);
                for(int i=0; i<SymbolTableVar.count(name_variable); i++){
                    if(it2->second.scope == name_function_declaration || it2->second.scope == "global"){
                        if(it2->second.scope == name_function_declaration){
                            aux_typ = it2->second.type;
                            i = SymbolTableVar.count(name_variable);
                        }
                        else {
                            aux_typ = it2->second.type;
                        }
                    }
                    it2++;
                }
                // Controlli parametro int
                if(aux_typ == "int" && (tipo == 1 || tipo == 3) ){
                    trad.open("python.py", ios::app);
                    trad << value;
                    trad.close();
                }
                if(aux_typ == "int" && tipo == 2 ){
                    trad.open("python.py", ios::app);
                    trad << "int(" << value<< ")";
                    trad.close();
                }
                if(aux_typ == "int" && tipo == 4) {
                    trad.open("python.py", ios::app);
                    trad << "int( " << value << " )";
                    trad.close();
                }

                // Controlli parametro float
                if(aux_typ == "float" && (tipo == 1 || tipo == 2 || tipo == 3)){
                    trad.open("python.py", ios::app);
                    trad << value;
                    trad.close();
                }
                if(aux_typ == "float" && tipo == 4 ){
                    trad.open("python.py", ios::app);
                    trad << "float( " << value << " )";
                    trad.close();
                }

                if(aux_typ == "char" && (tipo == 1 || tipo == 3) ){
                    trad.open("python.py", ios::app);
                    trad << value;
                    trad.close();
                }
                if(aux_typ == "char" && tipo == 2 ){
                    trad.open("python.py", ios::app);
                    trad << "int(" << value<< ")";
                    trad.close();
                }
                if(aux_typ == "char" && tipo == 4) {
                    trad.open("python.py", ios::app);
                    trad << "int( " << value << " )";
                    trad.close();
                }
                // Controlli parametro bool
                if(aux_typ == "bool" && tipo == 4){
                    trad.open("python.py", ios::app);
                    trad << "bool( " << value << " )";
                    trad.close();
                }
                if(aux_typ == "bool" && (tipo == 1 || tipo == 2 || tipo == 3)){
                    trad.open("python.py", ios::app);
                    trad << "bool( " << value << " )";
                    trad.close();
                }
            }else{
                if(!in){
                    trad.open("python.py", ios::app);
                    trad << value;
                    trad.close();
                }
            }
        }
    }
    nodo = 3;
}

void IfStmt :: translate(){
    nodo = 10;

    trad.open("python.py", ios::app);
    trad << "if( ";
    trad.close();

    if_condition = 1;
    condition->translate();
    if_condition = 0;

    trad.open("python.py", ios::app);
    trad << "):"<< endl;
    trad.close();

    for(int i=0; i<body_if.size();i++){
        if(ass_whil_if_name != "" && ass_whil_if_value != "" ){
            indentazione ++;
            indent(indentazione);
            trad.open("python.py", ios::app);
            trad << ass_whil_if_name << " = " << ass_whil_if_value<<endl;
            trad.close();
            indent(indentazione);
            body_if[i]->translate();
            trad.open("python.py", ios::app);
            trad << endl;
            trad.close();
            indentazione --;
        }
        else{
            indentazione ++;
            indent(indentazione);
            body_if[i]->translate();
            trad.open("python.py", ios::app);
            trad << endl;
            trad.close();
            indentazione --;
        }
    }

    if(body_else.size()!=0){
        indent(indentazione);
        trad.open("python.py", ios::app);
        trad << "else:"<< endl;
        trad.close();
    }
    else{
        if(ass_whil_if_name != "" && ass_whil_if_value != "" ){
            ass_whil_if_name = "";
            ass_whil_if_value = "";
        }
    }

    for(int i=0; i<body_else.size(); i++){
        if(ass_whil_if_name != "" && ass_whil_if_value != "" ){
            indentazione ++;
            indent(indentazione);
            trad.open("python.py", ios::app);
            trad << ass_whil_if_name << " = " << ass_whil_if_value<<endl;
            trad.close();
            ass_whil_if_name = "";
            ass_whil_if_value = "";
            indent(indentazione);
            body_else[i]->translate();
            trad.open("python.py", ios::app);
            trad << endl;
            trad.close();
            indentazione --;
        }else{
            indentazione ++;
            indent(indentazione);
            body_else[i]->translate();
            trad.open("python.py", ios::app);
            trad << endl;
            trad.close();
            indentazione --;
        }
    }

}

void WhileStmt :: translate(){
    nodo = 8;

    trad.open("python.py", ios::app);
    trad << "while( ";
    trad.close();

    while_condition=1;
    condition->translate();
    while_condition=0;

    trad.open("python.py", ios::app);
    trad << " ):"<< endl;
    trad.close();

    for(int i=0; i<body_while.size();i++){
        if(ass_whil_if_name != "" && ass_whil_if_value != "" ){
            indentazione ++;
            indent(indentazione);
            trad.open("python.py", ios::app);
            trad << ass_whil_if_name << " = " << ass_whil_if_value<<endl;
            trad.close();
            ass_whil_if_name = "";
            ass_whil_if_value = "";
            indent(indentazione);
            body_while[i]->translate();
            trad.open("python.py", ios::app);
            trad << endl;
            trad.close();
            indentazione --;
        }else{
            indentazione ++;
            indent(indentazione);
            body_while[i]->translate();
            trad.open("python.py", ios::app);
            trad << endl;
            trad.close();
            indentazione --;
        }
    }
}

void ReturnNode :: translate(){

    auto it = SymbolTableFunction.find(name_function_declaration);
    value->semantics();

    //In base al tipo di funzione, stabilisco cosa è possibile restituire

    //Funzioni di tipo intero e char
    if(it->second.type == "int" || it->second.type == "char") {
        //Se entro nel nodo int, restituisco quel valore
        if (nodo == 1) {
            trad.open("python.py", ios::app);
            trad << "return ";
            trad.close();
            value->translate();
        }
        //Se entro nel nodo float, restituisco l'intero di quel valore
        if (nodo == 2) {
            trad.open("python.py", ios::app);
            trad << "return int( ";
            trad.close();
            value->translate();
            trad.open("python.py", ios::app);
            trad << " )";
            trad.close();
        }
        //Se entro nel nodo char, restituisco ASCII di quel carattere
        if (nodo == 4){
            trad.open("python.py", ios::app);
            trad << "return ";
            trad.close();
            value->translate();
        }
        //Se entro nel nodo bool, restituisco 1 o 0 a seconda che il valore sia vero o falso
        if (nodo == 5){
            trad.open("python.py", ios::app);
            trad << "return int( ";
            trad.close();
            value->translate();
            trad.open("python.py", ios::app);
            trad << " )";
            trad.close();
        }
        // Se entro nel nodo identifier, devo verificare che tipo di variabile sto gestendo
        if (nodo == 3){
            // Variabile inter
            if (tipo == 1){
                trad.open("python.py", ios::app);
                trad << "return " ;
                trad.close();
                value->translate();
            }
            // Variabile float
            if (tipo == 2){
                trad.open("python.py", ios::app);
                trad << "return int( ";
                trad.close();
                value->translate();
                trad.open("python.py", ios::app);
                trad << " )";
                trad.close();
            }
            // Variabile char
            if (tipo == 3){
                trad.open("python.py", ios::app);
                trad << "return " ;
                trad.close();
                value->translate();
            }
            // Variabile bool
            if (tipo == 4){
                trad.open("python.py", ios::app);
                trad << "return int( ";
                trad.close();
                value->translate();
                trad.open("python.py", ios::app);
                trad << " )";
                trad.close();
            }
        }
    }

    //Funzioni di tipo float
    if(it->second.type == "float") {
        //Se entro nel nodo int, restituisco quel valore
        if (nodo == 1) {
            trad.open("python.py", ios::app);
            trad << "return float( ";
            trad.close();
            value->translate();
            trad.open("python.py", ios::app);
            trad << " )";
            trad.close();
        }
        //Se entro nel nodo float, restituisco quel valore
        if (nodo == 2) {
            trad.open("python.py", ios::app);
            trad << "return ";
            trad.close();
            value->translate();

        }
        //Se entro nel nodo char, restituisco ASCII di quel carattere
        if (nodo == 4){
            trad.open("python.py", ios::app);
            trad << "return float( ";
            trad.close();
            value->translate();
            trad.open("python.py", ios::app);
            trad << " )";
            trad.close();
        }
        //Se entro nel nodo bool, restituisco 1 o 0 a seconda che il valore sia vero o falso
        if (nodo == 5){
            trad.open("python.py", ios::app);
            trad << "return float( int( ";
            trad.close();
            value->translate();
            trad.open("python.py", ios::app);
            trad << " ) )";
            trad.close();
        }
        // Se entro nel nodo identifier, devo verificare che tipo di variabile sto gestendo
        if (nodo == 3){
            // Variabile inter
            if (tipo == 1){
                trad.open("python.py", ios::app);
                trad << "return float( ";
                trad.close();
                value->translate();
                trad.open("python.py", ios::app);
                trad << " )";
                trad.close();
            }
            // Variabile float
            if (tipo == 2){
                trad.open("python.py", ios::app);
                trad << "return ";
                trad.close();
                value->translate();
            }
            // Variabile char
            if (tipo == 3){
                trad.open("python.py", ios::app);
                trad << "return float( ";
                trad.close();
                value->translate();
                trad.open("python.py", ios::app);
                trad << " )";
                trad.close();
            }
            // Variabile bool
            if (tipo == 4){
                trad.open("python.py", ios::app);
                trad << "return float( int( ";
                trad.close();
                value->translate();
                trad.open("python.py", ios::app);
                trad << " ) )";
                trad.close();
            }
        }
    }

    //Funzioni di tipo bool
    if(it->second.type == "bool") {
        //Se entro nel nodo int, restituisco quel valore
        if (nodo == 1) {
            trad.open("python.py", ios::app);
            trad << "return bool( ";
            trad.close();
            value->translate();
            trad.open("python.py", ios::app);
            trad << " )";
            trad.close();
        }
        //Se entro nel nodo float, restituisco quel valore
        if (nodo == 2) {
            trad.open("python.py", ios::app);
            trad << "return bool( ";
            trad.close();
            value->translate();
            trad.open("python.py", ios::app);
            trad << " ) ";
            trad.close();

        }
        //Se entro nel nodo char, restituisco ASCII di quel carattere
        if (nodo == 4){
            trad.open("python.py", ios::app);
            trad << "return bool( ";
            trad.close();
            value->translate();
            trad.open("python.py", ios::app);
            trad << " )";
            trad.close();
        }
        //Se entro nel nodo bool, restituisco 1 o 0 a seconda che il valore sia vero o falso
        if (nodo == 5){
            trad.open("python.py", ios::app);
            trad << "return bool( ";
            trad.close();
            value->translate();
            trad.open("python.py", ios::app);
            trad << " ) ";
            trad.close();
        }
        // Se entro nel nodo identifier, devo verificare che tipo di variabile sto gestendo
        if (nodo == 3){
            // Variabile inter
            if (tipo == 1){
                trad.open("python.py", ios::app);
                trad << "return bool( ";
                trad.close();
                value->translate();
                trad.open("python.py", ios::app);
                trad << " )";
                trad.close();
            }
            // Variabile float
            if (tipo == 2){
                trad.open("python.py", ios::app);
                trad << "return bool( ";
                trad.close();
                value->translate();
                trad.open("python.py", ios::app);
                trad << " ) ";
                trad.close();
            }
            // Variabile char
            if (tipo == 3){
                trad.open("python.py", ios::app);
                trad << "return bool( ";
                trad.close();
                value->translate();
                trad.open("python.py", ios::app);
                trad << " )";
                trad.close();
            }
            // Variabile bool
            if (tipo == 4){
                trad.open("python.py", ios::app);
                trad << "return bool( ";
                trad.close();
                value->translate();
                trad.open("python.py", ios::app);
                trad << " ) ";
                trad.close();
            }
        }
    }

}

void PrintStmt :: translate(){
    if(!string_v){
        auto it= SymbolTableVar.find(name);

        for(int i=0; i<SymbolTableVar.count(name); i++) {
            if (it->second.scope == "global" || it->second.scope == name_function_declaration) {
                if (it->second.scope == name_function_declaration) {
                    aux_typ = it->second.type;
                    i = SymbolTableVar.count(name);
                } else {
                    aux_typ = it->second.type;
                }
            }
            it++;
        }

        if(specifiers == "\"%d\"" && (aux_typ == "int" || aux_typ == "char")){
            trad.open("python.py", ios::app);
            trad << "print ( " << name << " )";
            trad.close();
        }

        // Se ho specifier = %d e la variabile di tipo bool, devo stampare 1 se ho true, 0 se ho false
        if(specifiers == "\"%d\"" && aux_typ == "bool"){
            trad.open("python.py", ios::app);
            trad << "print ( " << "int( " << name << " )"  <<" )";
            trad.close();
        }

        if(specifiers == "\"%f\"" && aux_typ == "float"){
            trad.open("python.py", ios::app);
            trad << "print ( " <<  name <<" )";
            trad.close();
        }

        // Se ho specifier = %c e la variabile di tipo int, devo stampare il carattere appartenente a quel numero
        if(specifiers == "\"%c\"" && (aux_typ == "int" || aux_typ == "char")){
            trad.open("python.py", ios::app);
            trad << "print ( " << "chr( "<<  name << " )" <<" )";
            trad.close();
        }

        // Se ho specifier = %c e la variabile di tipo int, devo stampare il carattere appartenente a 0 o 1
        if(specifiers == "\"%c\"" && aux_typ == "bool"){
            trad.open("python.py", ios::app);
            trad << "print ( " << "chr( "<<  name << " )" <<" )";
            trad.close();
        }
    }
    else{
        string_v->translate();
    }

}

void ScanfStmt :: translate(){

    if(specifiers == "\"%d\""){
        trad.open("python.py", ios::app);
        trad << "case" << " = input()" << endl;
        indent2(indentazione);
        trad << "try:" << endl;
        indentazione++;
        indent2(indentazione);
        trad << name << " = int(case)" << endl;
        indentazione --;
        indent2(indentazione);
        trad << "except ValueError: " << endl;
        indentazione ++;
        indent2(indentazione);
        trad << name << " = " << name << endl;
        trad.close();
        indentazione = indentazione - 1;
    }

    if(specifiers == "\"%f\""){
        trad.open("python.py", ios::app);
        trad << "case" << " = input()" << endl;
        indent2(indentazione);
        trad << "try:" << endl;
        indentazione++;
        indent2(indentazione);
        trad << name << " = float(case)" << endl;
        indentazione --;
        indent2(indentazione);
        trad << "except ValueError: " << endl;
        indentazione ++;
        indent2(indentazione);
        trad << name << " = " << name << endl;
        trad.close();
        indentazione = indentazione - 1;
    }

    if(specifiers == "\"%c\"")  {
        trad.open("python.py", ios::app);
        trad << "case" << " = input()" << endl;
        trad.close();
        indent(indentazione);
        trad.open("python.py", ios::app);
        trad << name << " = ord(case[0])";
        trad.close();
    }
}

void BinaryOperator :: translate() {
    bool in_if = false;
    //Gestisco il caso in cui ho un'assegnazione nella condizione del while
    if((nodo == 8 && while_condition == 1 && assignment_in == 1 && !condition) || (nodo == 10 && if_condition == 1 && assignment_in == 1 && !condition)){
        condition = true;
        trad.open("python.py", ios::app);
        trad << "struct(";
        trad.close();

        //Se non è stata ancora trascritta la funzione di supporto lo faccio
        if(new_file){
            new_file = false;
            trad.open("aux.py", ios::app);
            trad << "def struct(x):"<< endl;
            trad << "\t if(x == 0):"<< endl;
            trad << "\t \t return False" << endl;
            trad << "\t else:" << endl;
            trad << "\t \t return True"<< endl;
            trad.close();
        }
        utile = true;
        nodo = 9;
        if (!lexpr) {
            switch (op) {
                case T_NOT  :
                    trad.open("python.py", ios::app);
                    trad << " not ";
                    trad.close();
            }
            rexpr->translate();
        }

        if (lexpr) {
            trad.open("python.py", ios::app);
            trad << "(";
            trad.close();

            lexpr->translate();
            switch (op) {
                case T_PLUS    :
                    trad.open("python.py", ios::app);
                    trad << " + ";
                    trad.close();
                    break;
                case T_MINUS    :
                    trad.open("python.py", ios::app);
                    trad << " - ";
                    trad.close();
                    break;
                case T_MULTIPLY    :
                    trad.open("python.py", ios::app);
                    trad << " * ";
                    trad.close();
                    break;
                case T_DIVIDE    :
                    trad.open("python.py", ios::app);
                    trad << " / ";
                    trad.close();
                    break;
                case T_LT    :
                    trad.open("python.py", ios::app);
                    trad << " < ";
                    trad.close();
                    break;
                case T_GT    :
                    trad.open("python.py", ios::app);
                    trad << " > ";
                    trad.close();
                    break;
                case T_GE    :
                    trad.open("python.py", ios::app);
                    trad << " >= ";
                    trad.close();
                    break;
                case T_LE    :
                    trad.open("python.py", ios::app);
                    trad << " <= ";
                    trad.close();
                    break;
                case T_EQUAL    :
                    trad.open("python.py", ios::app);
                    trad << " == ";
                    trad.close();
                    break;
                case T_NOTEQUAL    :
                    trad.open("python.py", ios::app);
                    trad << " != ";
                    trad.close();
                    break;
                case T_AND    :
                    trad.open("python.py", ios::app);
                    trad << " and ";
                    trad.close();
                    break;
                case T_OR    :
                    trad.open("python.py", ios::app);
                    trad << " or ";
                    trad.close();
                    break;
            }

            nodo = 9;
            rexpr->translate();
            trad.open("python.py", ios::app);
            trad << ")";
            trad.close();
            utile = false;
        }

        trad.open("python.py", ios::app);
        trad << ")";
        trad.close();
        condition = false;
    }
    else {
        //Gestisco tutti gli altri casi
        if (nodo != 9 && tipo == 1) {
            trad.open("python.py", ios::app);
            trad << "int( ";
            trad.close();
            in_if = true;
        }
        if (nodo != 9 && tipo == 4) {
            trad.open("python.py", ios::app);
            trad << "bool( ";
            trad.close();
            in_if = true;
        }

        nodo = 9;
        if (!lexpr) {
            switch (op) {
                case T_NOT  :
                    trad.open("python.py", ios::app);
                    trad << " not ";
                    trad.close();
            }
            rexpr->translate();
        }

        if (lexpr) {
            trad.open("python.py", ios::app);
            trad << "(";
            trad.close();

            lexpr->translate();
            switch (op) {
                case T_PLUS    :
                    trad.open("python.py", ios::app);
                    trad << " + ";
                    trad.close();
                    break;
                case T_MINUS    :
                    trad.open("python.py", ios::app);
                    trad << " - ";
                    trad.close();
                    break;
                case T_MULTIPLY    :
                    trad.open("python.py", ios::app);
                    trad << " * ";
                    trad.close();
                    break;
                case T_DIVIDE    :
                    trad.open("python.py", ios::app);
                    trad << " / ";
                    trad.close();
                    break;
                case T_LT    :
                    trad.open("python.py", ios::app);
                    trad << " < ";
                    trad.close();
                    break;
                case T_GT    :
                    trad.open("python.py", ios::app);
                    trad << " > ";
                    trad.close();
                    break;
                case T_GE    :
                    trad.open("python.py", ios::app);
                    trad << " >= ";
                    trad.close();
                    break;
                case T_LE    :
                    trad.open("python.py", ios::app);
                    trad << " <= ";
                    trad.close();
                    break;
                case T_EQUAL    :
                    trad.open("python.py", ios::app);
                    trad << " == ";
                    trad.close();
                    break;
                case T_NOTEQUAL    :
                    trad.open("python.py", ios::app);
                    trad << " != ";
                    trad.close();
                    break;
                case T_AND    :
                    trad.open("python.py", ios::app);
                    trad << " and ";
                    trad.close();
                    break;
                case T_OR    :
                    trad.open("python.py", ios::app);
                    trad << " or ";
                    trad.close();
                    break;
            }

            nodo = 9;
            rexpr->translate();
            trad.open("python.py", ios::app);
            trad << ")";
            trad.close();
        }

        if (in_if) {
            trad.open("python.py", ios::app);
            trad << " )";
            trad.close();
        }
    }
}