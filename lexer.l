%{

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "types.h"
#include "y.tab.h"

extern int yylex (void);

int yyerror(const char* message);

extern int freshNewline;

%}

digit		[0-9]
underscore	[_]
alpha		[A-Za-z]
bad_integer	0{digit}+
integer		(0|[1-9]{digit}*)
identifier	({underscore}+({alpha}|{digit})+|{alpha})({underscore}|{digit}|{alpha})*
whitespace	[ \t]+
double		({digit}+)?\.{digit}*

%%

"+"			return PLUS;
"-"			return MINUS;
"*"			return MULTIPLY;
"/"			return DIVIDE;

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
"end"		return END;

"def"		return DEF;
"local"		return LOCAL;
"return"	return RETURN;

"="			return ASSIGN;

"("			return LPAREN;
")"			return RPAREN;

";"			return DELIMITER;
","			return COMMA;

"\n" {
	if (freshNewline) {
		printf("?- ");
		freshNewline = 0;
	} else {
		printf("> ");
	}
}

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
	return INTEGER_VALUE;
}

{double} {
	double val = atof(yytext);
	yylval.doubleVal = val;
	return DOUBLE_VALUE;
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

{whitespace} {
}

. {
	return LEX_ERROR;
}

%%
