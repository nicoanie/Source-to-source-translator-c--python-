/*
 *      - - PARSER FOR PYTHON TO C COMPILER - -
 */

%{
	#include <string>
	#include <vector>
	#include <iostream>
	#include <cstdio>
	#include <algorithm>


	#include "node.cpp"

	using namespace std;

	Node* programBlock;

	bool err=false;

	vector<Node*> global;
	vector<Node*> funzioni;



	/* variabile aggiunta per evitare print di errori che generano loop */
	int err_line;


	extern int yylineno;
	extern int yylex();

	void yyerror(const char *s) { printf("line: %d - %s\n", yylineno, s); }
%}

%error-verbose

%union {

	Node* node;
	Vec* vec;

	char *a;
	int token;

}

	/* Declaration of tokens */
%token <a> T_TYPE T_IDENT T_INT T_FLOAT T_BOOL T_CHAR
%token <token> T_PLUS T_MINUS T_MULTIPLY T_DIVIDE T_ASSIGN
%token <token> T_LT T_GT T_EQUAL T_LE T_GE T_NOTEQUAL
%token <token> T_AND T_OR T_NOT
%token <token> T_RETURN T_WHILE T_ELSE T_IF T_PRINTF T_SCANF

%type <node> Program
%type <node> Primary
%type <node> Expression
%type <node> Assignment
%type <node> ReturnNode
%type <node> IfStmt
%type <node> WhileStmt
%type <node> PrintfStmt
%type <node> ScanfStmt
%type <node> FunctionCall


%type <node> VariableDeclaration
%type <node> ExternalDeclaration
%type <node> VariableAssignment

%type <node> FunctionDeclaration

%type <vec> ArgumentsDefinition
%type <vec> ExternalContent
%type <vec> FunctionContent
%type <vec> CallArguments




%right T_ASSIGN T_NOT
%left T_AND T_OR
%left T_LT T_GT T_EQUAL T_NOTEQUAL T_GE T_LE
%left T_PLUS T_MINUS
%left T_MULTIPLY T_DIVIDE

%start Program

%%

Program
	: ExternalDeclaration 								{ if(err) exit(1); programBlock = $1; }
    ;

ExternalDeclaration
    : ExternalContent                                   { reverse(global.begin(), global.end()); reverse(funzioni.begin(), funzioni.end()); $$ = new ExternalDec(yylineno, global, funzioni); global.clear(); }
    ;

ExternalContent
	: FunctionDeclaration								{ funzioni.push_back($1); }
	| FunctionDeclaration ExternalContent   			{ funzioni.push_back($1); }
	| VariableDeclaration ';' ExternalContent   		{ global.push_back($1); }
	;

FunctionDeclaration
	: T_TYPE T_IDENT '(' ArgumentsDefinition ')' '{' FunctionContent '}'		{ $$ = new FuncDecl(yylineno, $1, $2, $4->nodes, $7->nodes); }
	| T_TYPE T_IDENT '(' ')' '{' FunctionContent '}'							{ $$ = new FuncDecl(yylineno, $1, $2, $6->nodes); }
	;

VariableDeclaration
	: T_TYPE T_IDENT								{ $$ = new VariableDec(yylineno, $1, $2); }
	;

VariableAssignment
	: T_TYPE T_IDENT T_ASSIGN Expression			{ $$ = new VariableDec(yylineno, $1, $2, $4); }
	| T_TYPE T_IDENT T_ASSIGN FunctionCall			{ $$ = new VariableDec(yylineno, $1, $2, $4); }
	;

Primary
	: T_INT 			{ $$ = new IntegerNode(yylineno, $1); }
	| T_FLOAT 			{ $$ = new FloatNode(yylineno, $1); }
	| T_CHAR 			{ $$ = new CharNode(yylineno, $1); }
	| T_BOOL 			{ $$ = new BoolNode(yylineno, $1); }
	| T_IDENT			{ $$ = new IdentifierNode(yylineno, $1); }
	;

ArgumentsDefinition
	: VariableDeclaration 							{ $$ = new Vec(); $$->push($1); }
	| ArgumentsDefinition ',' VariableDeclaration	{ $$->push($3); }
	;

FunctionContent
	: /* Blank */									{ $$ = new Vec(); }
	| FunctionContent VariableDeclaration ';'		{ $$->push($2); }
	| FunctionContent VariableAssignment ';' 		{ $$->push($2); }
	| FunctionContent FunctionCall ';' 			    { $$->push($2); }
	| FunctionContent PrintfStmt ';' 				{ $$->push($2); }
	| FunctionContent ScanfStmt ';' 				{ $$->push($2); }
	| FunctionContent IfStmt 						{ $$->push($2); }
	| FunctionContent WhileStmt  					{ $$->push($2); }
	| FunctionContent ReturnNode ';' 				{ $$->push($2); }
	| FunctionContent Assignment ';' 				{ $$->push($2); }
	;

FunctionCall
	: T_IDENT '(' CallArguments ')'			{ $$ = new FunctionCall(yylineno, $1, $3->nodes); }
	| T_IDENT '(' ')'						{ $$ = new FunctionCall(yylineno, $1); }
	;

CallArguments
	: Expression							{ $$ = new Vec(); $$->push($1); }
	| CallArguments ',' Expression			{ $$->push($3); }
	;

PrintfStmt
	: T_PRINTF '(' '"' T_IDENT '"' ')'			{ $$ = new PrintStmt(yylineno, $4); }
	;

ScanfStmt
    : T_SCANF '(' '"' T_IDENT '"' ')'			{ $$ = new ScanfStmt(yylineno, $4); }
    ;

IfStmt
	: T_IF '(' Expression ')' '{' FunctionContent '}' 										{ $$ = new IfStmt (yylineno, $3, $6->nodes); }
	| T_IF '(' Expression ')' '{' FunctionContent '}' T_ELSE '{' FunctionContent '}'		{ $$ = new IfStmt (yylineno, $3, $6->nodes, $10->nodes); }
	;

WhileStmt
	: T_WHILE '(' Expression ')' '{' FunctionContent '}'		{ $$ = new WhileStmt (yylineno, $3, $6->nodes); }
	;

ReturnNode
	: T_RETURN Expression 										{ $$ = new ReturnNode (yylineno, $2); }
	;

Assignment
	: T_IDENT T_ASSIGN Expression								{ $$ = new Assignment (yylineno, $1, $3); }
	| T_IDENT T_ASSIGN FunctionCall								{ $$ = new Assignment (yylineno, $1, $3); }
	;

Expression
	: Primary								{ $$ = $1; }
	| '(' Expression ')'					{ $$ = $2; }
	| Expression T_PLUS Expression 			{ $$ = new BinaryOperator(yylineno, $2, $1, $3); }
	| Expression T_MINUS Expression 		{ $$ = new BinaryOperator(yylineno, $2, $1, $3); }
	| Expression T_MULTIPLY Expression 		{ $$ = new BinaryOperator(yylineno, $2, $1, $3); }
	| Expression T_DIVIDE Expression 		{ $$ = new BinaryOperator(yylineno, $2, $1, $3); }
	| Expression T_LT Expression 			{ $$ = new BinaryOperator(yylineno, $2, $1, $3); }
	| Expression T_GT Expression 			{ $$ = new BinaryOperator(yylineno, $2, $1, $3); }
	| Expression T_GE Expression 			{ $$ = new BinaryOperator(yylineno, $2, $1, $3); }
	| Expression T_LE Expression 			{ $$ = new BinaryOperator(yylineno, $2, $1, $3); }
	| Expression T_AND Expression 			{ $$ = new BinaryOperator(yylineno, $2, $1, $3); }
	| Expression T_OR Expression 			{ $$ = new BinaryOperator(yylineno, $2, $1, $3); }
	| Expression T_NOT Expression 			{ $$ = new BinaryOperator(yylineno, $2, $1, $3); }
	| Expression T_EQUAL Expression 		{ $$ = new BinaryOperator(yylineno, $2, $1, $3); }
	| Expression T_NOTEQUAL Expression 		{ $$ = new BinaryOperator(yylineno, $2, $1, $3); }
	| '(' error ')'							{ if(err_line==yylineno) exit(1); err_line=yylineno; yyerrok;}
	;

	/* TODO:
	         Verificare che Operatore logico NOT abbia precedenza a destra
	         Vedere come far funzionare SCANF
			 PRINTF dovrebbe essere sbagliato
			 Vedere ExternalDeclaration se va bene così
			 Gestione degli Errori nel parser
	 */
