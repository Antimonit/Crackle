%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "headers/types.h"
#include "y.tab.h"

extern int yyerror(const char* message);

%}

digit		[0-9]
underscore	[_]
alpha		[A-Za-z]
bad_integer	0{digit}+
integer		(0|[1-9]{digit}*)
identifier	({underscore}+({alpha}|{digit})+|{alpha})({underscore}|{digit}|{alpha})*
whitespace	[ \t]+
double		({digit}+)?\.{digit}*
string      \"(\\.|[^\\"])*\"

%%

"+"			return '+';
"-"			return '-';
"*"			return '*';
"/"			return '/';
"%"			return '%';

"<"			return LT;
"<="		return LE;
">"			return GT;
">="		return GE;
"=="		return EQ;
"!="		return NE;

"true"		return TRUE;
"false"		return FALSE;
"||"		return OR;
"&&"		return AND;
"!"			return NEG;

"while"		return WHILE;
"if"		return IF;
"else"		return ELSE;
"do"        return DO;
"for"       return FOR;

"int"       return INT;
"double"    return DOUBLE;
"bool"      return BOOL;
"string"    return STRING;

"var"       return VAR;
"fun"		return FUN;
"return"	return RETURN;

"print"		return PRINT;
"println"   return PRINTLN;

"="			return '=';

"("			return '(';
")"			return ')';

"{"			return '{';
"}"			return '}';

":"         return ':';
";"			return ';';
","			return ',';
"."         return '.';


{bad_integer} {
	yyerror("bad integer\n");
}

{integer} {
	if (yyleng > 10) {
		yyleng = 10;
		yytext[10] = 0;
	}
	int val = atoi(yytext);
	yylval.intVal = val;
	return INT_VALUE;
}

{double} {
	double val = atof(yytext);
	yylval.doubleVal = val;
	return DOUBLE_VALUE;
}

{string} {
	char* text = (char*) malloc(sizeof(char) * (yyleng - 1));
	// strip leading and trailing quotation marks
	strncpy(text, yytext+1, yyleng - 1);
	text[yyleng-2] = '\0';
    yylval.stringVal = text;
    return STRING_VALUE;
}

{identifier} {
	if (yyleng > 16) {
		yyleng = 16;
		yytext[16] = 0;
	}

	char* id = (char*) malloc(sizeof(char) * (yyleng + 1));
	strncpy(id, yytext, yyleng + 1);
	yylval.identifier = id;
	return IDENTIFIER;
}

"\n" {
}

{whitespace} {
}

. {
	return LEX_ERROR;
}

%%
