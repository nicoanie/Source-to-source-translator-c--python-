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

[();,{}. & '] { return yytext[0]; }

	/* Identificator and data type */

"int"|"float"|"bool"|"char"		{yylval.a = strdup(yytext); return T_TYPE;}

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
"scanf"		{return T_SCANF;}

	/* Selection and iteration */
"if"		{return T_IF;}
"else"		{return T_ELSE;}
"while"		{return T_WHILE;}
"return"	{return T_RETURN;}
"#include"  {return T_INCLUDE;}
".h"        {return T_SUF; }

    /* Reserved Word */
"auto"  	{}
"break"	    {}
"case"	    {}
"const"	    {}
"continue"	{}
"default"	{}
"do"	    {}
"double"	{}
"enum"	    {}
"extern"	{}
"for"	    {}
"goto"	    {}
"long"	    {}
"register"	{}
"short"	    {}
"signed"	{}
"sizeof"	{}
"static"	{}
"struct"	{}
"switch"	{}
"typedef"	{}
"union"		{}
"unsigned"	{}
"void"		{}
"volatile"	{}
"_Packed"	{}

	/* Identificator and data type */

"true"|"false" 			{yylval.a = strdup(yytext); return T_BOOL;}

["](%[a-zA-Z_][a-zA-Z0-9_]*)["]   {yylval.a = strdup(yytext); return T_SPECIFIER;}

[a-zA-Z_][a-zA-Z0-9_]*  	{yylval.a = strdup(yytext); return T_IDENT;}

([0]|[1-9][0-9]*)			{ yylval.a = strdup(yytext); return T_INT;}

([0-9]*)[.]?[0-9]+ 			{ yylval.a = strdup(yytext); return T_FLOAT;}

[']([^"\\\r\n]|[\\][btnfr"'\\])['] 	{ yylval.a = strdup(yytext); return T_CHAR;}

["]([^"\\\r\n]|[\\][btnfr"'\\])*["] { yylval.a = strdup(yytext); return T_STR;}

%%