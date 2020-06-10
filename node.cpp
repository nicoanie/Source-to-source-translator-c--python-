#include <iostream>
#include <string>
#include <vector>


using namespace std;
/* ------------------------------ Dichiarazione classe principale Nodo ------------------------------- */

class Node
{
public:
    int line;
    string type;
    string name;
    string valore;

    virtual ~Node() = default;
    virtual void semantics() = 0;
    virtual void translate() = 0;
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
    void semantics();
    void translate();

    IntegerNode(int line, char* value){
        this->line = line;
        this->value = atoi(value);
    }
};

class FloatNode : public Node
{
public:
    float value;
    void semantics();
    void translate();

    FloatNode(int line, char* value){
        this->line = line;
        this->value = atof(value);
    }
};

class CharNode : public Node
{
public:
    char value;
    void semantics();
    void translate();

    CharNode(int line, char* value){
        this->line = line;
        this->value = value[1];
    }
};

class BoolNode : public Node
{
public:
    string value;
    void semantics();
    void translate();

    BoolNode(int line, char* value):value(value){
        this->line = line;
    }
};

class IdentifierNode : public Node
{
public:
    string value;
    void semantics();
    void translate();

    IdentifierNode(int line, char* value){
        this->line = line;
        this->value = value;
    }
};

class StringNode : public Node
{
public:
    string value;
    void semantics();
    void translate();

    StringNode(int line, char* value):value(value){
        this->line = line;
    }

};
/* --------------------------------------------------------------------------------------------------- */

class ExternalDec : public Node
{
    vector<Node*> variables;
    vector<Node*> function;
    vector<Node*> librerie;
    void semantics();
    void translate();

public:
    ExternalDec(int line, vector<Node*> variables, vector<Node*> function, vector<Node*> librerie):variables(variables),function(function), librerie(librerie){
        this->line = line;
    }
};

class LibDec : public Node
{
    void semantics();
    void translate();

public:
    LibDec(int line, char* name){
        this->line = line;
        this->name = name;
    }
};

class VariableDec : public Node
{
public:
    Node* value;
    void semantics();
    void translate();

    VariableDec(int line, char* type, char* name){
        this->line = line;
        this->type=type;
        this->name=name;
        this->valore = "NULL";
    }
    VariableDec(int line, char* type, char* name, Node* value):value(value){
        this->line = line;
        this->type=type;
        this->name=name;
        this->valore = "NOT NULL";
    }
};


class Assignment : public Node
{
public:
    Node* value;
    void semantics();
    void translate();

    Assignment(int line, char* name, Node* value):value(value){
        this->line = line;
        this->name = name;
    }

};

class FuncDecl : public Node
{
public:
    vector<Node*> args;
    vector<Node*> body;
    void semantics();
    void translate();

    FuncDecl(int line, char* type, char* name, vector<Node*> args, vector<Node*> body):args(args),body(body){
        this->line = line;
        this->type = type;
        this->name = name;
    }

    FuncDecl(int line, char* type, char* name, vector<Node*> body):body(body){
        this->line = line;
        this->type = type;
        this->name = name;
    }

};

class FunctionCall : public Node
{
public:
    vector<Node*> args;
    void semantics();
    void translate();

    FunctionCall(int line, char* name, vector<Node*> args):args(args){
        this->line = line;
        this->name = name;
    }

    FunctionCall(int line, char* name){
        this->line = line;
        this->name = name;
    }

};

/* ------------------------------------- Selection and iteration ------------------------------------- */
class IfStmt : public Node
{
public:
    Node* condition;
    vector<Node*> body_if;
    vector<Node*> body_else;
    void semantics();
    void translate();

    IfStmt(int line, Node* condition, vector<Node*> body_if):condition(condition),body_if(body_if){
        this->line = line;
    }

    IfStmt(int line, Node* condition, vector<Node*> body_if, vector<Node*> body_else):condition(condition),body_if(body_if),body_else(body_else){
        this->line = line;
    }

};

class WhileStmt : public Node
{
public:
    Node* condition;
    vector<Node*> body_while;
    void semantics();
    void translate();

    WhileStmt(int line, Node* condition, vector<Node*> body_while):condition(condition),body_while(body_while){
        this->line = line;
    }

};

class ReturnNode : public Node
{
public:
    Node* value;
    void semantics();
    void translate();

    ReturnNode(int line, Node* value):value(value){
        this->line = line;
    }

};


class PrintStmt : public Node
{
public:
    string specifiers;
    StringNode* string_v;
    void semantics();
    void translate();
    void print_ast();

    PrintStmt(int line, char* specifiers , char* variable):specifiers(specifiers){
        this->line = line;
        this->name = variable;
    }

    PrintStmt(int line,  StringNode* string_v):string_v(string_v){
        this->line = line;
    }

};

class ScanfStmt : public Node
{
public:
    string specifiers;
    void semantics();
    void translate();
    void print_ast();

    ScanfStmt(int line, char* specifiers , char* variable):specifiers(specifiers){
        this->line = line;
        this->name = variable;
    }

};

/* ------------------------------------- Operatori Binari ------------------------------------- */

class BinaryOperator : public Node
{
public:
    int op;
    Node*	lexpr;
    Node*	rexpr;
    void semantics();
    void translate();

    BinaryOperator(int line, int op, Node* lexpr, Node* rexpr):op(op),lexpr(lexpr),rexpr(rexpr) {
        this->line = line;

    }

    BinaryOperator(int line, int op, Node* rexpr):op(op),rexpr(rexpr) {
        this->line = line;

    }
};

/* -------------------------------------------------------------------------------------------- */



