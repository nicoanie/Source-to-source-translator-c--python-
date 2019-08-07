#include <iostream>
#include <string>
#include <vector>
#include "Value.h"

using namespace std;
/* ------------------------------ Dichiarazione classe principale Nodo ------------------------------- */

class Node
{
public:
    int line;
    string type;
    string name;
    virtual ~Node() = default;
    virtual Struttura* semantics() = 0;
};

class Vec
{
public:
    vector<Node*> nodes;
    void push(Node* a){
        nodes.push_back(a);
    }
};

/* --------------------------------------------------------------------------------------------------- */

/* -------------------------- Dichiarazione classi Identificator e Data Type ------------------------- */
class IntegerNode : public Node
{
public:
    int value;
    virtual Struttura* semantics();

    IntegerNode(int line, char* value){
        this->line = line;
        this->value = atoi(value);

        //TODO: Vedere cosa bisogna fare dopo
    }
};

class FloatNode : public Node
{
public:
    float value;
    virtual Struttura* semantics();

    FloatNode(int line, char* value){
        this->line = line;
        this->value = atof(value);
        cout << this->value << endl;

        //TODO: Vedere cosa bisogna fare dopo
    }
};

class CharNode : public Node
{
public:
    char value;
    virtual Struttura* semantics();

    CharNode(int line, char* value){
        this->line = line;
        this->value = value[1];
        cout << this->value << endl;

        //TODO: Vedere cosa bisogna fare dopo
    }
};

class BoolNode : public Node
{
public:
    string value;
    virtual Struttura* semantics();

    BoolNode(int line, char* value):value(value){
        this->line = line;
        cout << this->value << endl;

        //TODO: Vedere cosa bisogna fare dopo
    }
};

class IdentifierNode : public Node
{
public:
    string value;
    virtual Struttura* semantics();

    IdentifierNode(int line, char* value){
        this->line = line;
        this->value = value;
        cout << this->value << endl;

        //TODO: Vedere cosa bisogna fare dopo
    }
};
/* --------------------------------------------------------------------------------------------------- */

class VariableDec : public Node
{
public:
    Node* value;
    virtual Struttura* semantics();

    VariableDec(int line, char* type, char* name){
        this->line = line;
        this->type=type;
        this->name=name;

        //TODO: Nel caso sia anche assegnato il valore, verificare che corrisponda al tipo
    }
    VariableDec(int line, char* type, char* name, Node* value):value(value){
        this->line = line;
        this->type=type;
        this->name=name;

        //TODO: Nel caso sia anche assegnato il valore, verificare che corrisponda al tipo
    }
};


class Assignment : public Node
{
public:
    Node* value;
    virtual Struttura* semantics();

    Assignment(int line, char* name, Node* value):value(value){
        this->line = line;
        this->name = name;

        //TODO: Vedere come utilizzare l'espressione e cosa deve contenere il nodo
    }

};

class FuncDecl : public Node
{
public:
    vector<Node*> args;
    vector<Node*> body;
    virtual Struttura* semantics();

    FuncDecl(int line, char* type, char* name, vector<Node*> args, vector<Node*> body):args(args),body(body){
        this->line = line;
        this->type = type;
        this->name = name;
        //TODO: Mi serve anche il numero di variabili dichiarate per quando richiamo la funzione
    }

    FuncDecl(int line, char* type, char* name, vector<Node*> body):body(body){
        this->line = line;
        this->type = type;
        this->name = name;
        //TODO: Mi serve anche il numero di variabili dichiarate per quando richiamo la funzione
    }

};

class FunctionCall : public Node
{
public:
    vector<Node*> args;
    virtual Struttura* semantics();

    FunctionCall(int line, char* name, vector<Node*> args):args(args){
        this->line = line;
        this->name = name;
        cout << "Name: "<< name<< "||" << "line: "<< line<< endl;
        //TODO
    }

    FunctionCall(int line, char* name){
        this->line = line;
        this->name = name;
        cout << "Name: "<< name<< "||" << "line: "<< line<< endl;

        //TODO
    }

};

/* ------------------------------------- Selection and iteration ------------------------------------- */
class IfStmt : public Node
{
public:
    Node* condition;
    vector<Node*> body_if;
    vector<Node*> body_else;
    virtual Struttura* semantics();

    IfStmt(int line, Node* condition, vector<Node*> body_if):condition(condition),body_if(body_if){
        this->line = line;

        //TODO: Vedere come utilizzare la condizione e cosa deve contenere il nodo
    }

    IfStmt(int line, Node* condition, vector<Node*> body_if, vector<Node*> body_else):condition(condition),body_if(body_if),body_else(body_else){
        this->line = line;

        //TODO: Vedere come utilizzare la condizione e cosa deve contenere il nodo
    }

};

class WhileStmt : public Node
{
public:
    Node* condition;
    vector<Node*> body_while;
    virtual Struttura* semantics();

    WhileStmt(int line, Node* condition, vector<Node*> body_while):condition(condition),body_while(body_while){
        this->line = line;

        //TODO: Vedere come utilizzare la condizione e cosa deve contenere il nodo
    }

};

class ReturnStmt : public Node
{
public:
    Node* value;
    virtual Struttura* semantics();

    ReturnStmt(int line, Node* value):value(value){
        this->line = line;

        //TODO: Vedere come gestire il valore
    }

};


class PrintStmt : public Node
{
public:
    Node* value;
    virtual Struttura* semantics();

    PrintStmt(int line, char* value){
        this->line = line;

        //TODO: Capire come gestire il tutto
    }

};

class ScanfStmt : public Node
{
public:
    Node* value;
    virtual Struttura* semantics();

    ScanfStmt(int line, char* value){
        this->line = line;

        //TODO: Capire come gestire il tutto
    }

};

/* ------------------------------------- Operatori Binari ------------------------------------- */

class BinaryOperator : public Node
{
public:
    int operatore;
    Node*	lexpr;
    Node*	rexpr;
    virtual Struttura* semantics();

    BinaryOperator(int line, int operatore, Node* lexpr, Node* rexpr):operatore(operatore),lexpr(lexpr),rexpr(rexpr) {
        this->line = line;

    }
    /* TODO: Vedere come gestire*/
};

/* -------------------------------------------------------------------------------------------- */

/* ------------------------------------- Per creare l'albero ------------------------------------- */

class ExternalDec : public Node
{
    vector<Node*> variables;
    vector<Node*> function;
    virtual Struttura* semantics();

public:
    ExternalDec(int line, vector<Node*> variables, vector<Node*> function):variables(variables),function(function){
        this->line = line;
    }
    /* TODO: Vedere come gestire*/
};

/* -------------------------------------------------------------------------------------------- */