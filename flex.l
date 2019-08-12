/* LEXER FOR C TO PYTHON COMPILER */

%{

	#include <string>
	#include <iostream>
	#include <cstdio>

	#include "node.cpp"
	#include "parser.hpp"
%}

using namespace std;

%option noyywrap
%option yylineno

%%

[ \t]	{}
[\n] { yylineno+1; }

[();,{}.] { return yytext[0]; }

	/* Identificator and data type */

"int"|"float"|"boolean"|"char"		{yylval.a = strdup(yytext); return T_TYPE;}

"//".*	{}

	/* Mathematical operators */
"+"		{return T_PLUS;}
"-"		{return T_MINUS;}
"*"		{return T_MULTIPLY;}
"/"		{return T_DIVIDE;}

	/* Conditional operators */
"<"		{return T_LT;}
">"		{return T_GT;}
"=="	{return T_EQUAL;}
"<="	{return T_LE;}
">="	{return T_GE;}
"!="	{return T_NOTEQUAL;}

"="		{return T_ASSIGN;}

	/* Logical operators */
"&&"	{return T_AND;}
"||"	{return T_OR;}
"!"		{return T_NOT;}

	/* Built-in function */
"printf"	{return T_PRINTF;}
	/* "scanf"		{return T_SCANF;} */

	/* Selection and iteration */
"if"		{return T_IF;}
"else"		{return T_ELSE;}
"while"		{return T_WHILE;}
"return"	{return T_RETURN;}

"auto" 	{printf("Reserved word: %s\n", yytext);}
"break"	{printf("Reserved word: %s\n", yytext);}
"case"	{printf("Reserved word: %s\n", yytext);}
"const"	{printf("Reserved word: %s\n", yytext);}
"continue"	{printf("Reserved word: %s\n", yytext);}
"default"	{printf("Reserved word: %s\n", yytext);}
"do"	{printf("Reserved word: %s\n", yytext);}
"double"	{printf("Reserved word: %s\n", yytext);}
"enum"	{printf("Reserved word: %s\n", yytext);}
"extern"	{printf("Reserved word: %s\n", yytext);}
"for"	{printf("Reserved word: %s\n", yytext);}
"goto"	{printf("Reserved word: %s\n", yytext);}
"long"	{printf("Reserved word: %s\n", yytext);}
"register"	{printf("Reserved word: %s\n", yytext);}
"short"	{printf("Reserved word: %s\n", yytext);}
"signed"	{printf("Reserved word: %s\n", yytext);}
"sizeof"	{printf("Reserved word: %s\n", yytext);}
"static"	{printf("Reserved word: %s\n", yytext);}
"struct"	{printf("Reserved word: %s\n", yytext);}
"switch"	{printf("Reserved word: %s\n", yytext);}
"typedef"	{printf("Reserved word: %s\n", yytext);}
"union"		{printf("Reserved word: %s\n", yytext);}
"unsigned"	{printf("Reserved word: %s\n", yytext);}
"void"		{printf("Reserved word: %s\n", yytext);}
"volatile"	{printf("Reserved word: %s\n", yytext);}
"_Packed"	{printf("Reserved word: %s\n", yytext);}

	/* Identificator and data type */
"true"|"false" 			{yylval.a = strdup(yytext); return T_BOOL;}

[a-zA-Z_][a-zA-Z0-9_]*  	{yylval.a = strdup(yytext); return T_IDENT;}

([0]|[1-9][0-9]*)			{ yylval.a = strdup(yytext); return T_INT;}

([0-9]*)[.]?[0-9]+ 			{ yylval.a = strdup(yytext); return T_FLOAT;}


[']([^"\\\r\n]|[\\][btnfr"'\\])['] 	{ yylval.a = strdup(yytext); return T_CHAR;}

%%