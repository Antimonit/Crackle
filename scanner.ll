%{
#include <string>
#include "scanner.hpp"

extern int yyerror(const char* message);

#undef  YY_DECL
#define YY_DECL int MC::Scanner::yylex(MC::Parser::semantic_type * const lval, MC::Parser::location_type *loc)

using token = MC::Parser::token;

/* define yyterminate as this instead of NULL */
/* #define yyterminate() return( token::END ) */

/* msvc2010 requires that we exclude this header file. */
#define YY_NO_UNISTD_H

/* update location on matching */
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);

%}

%option debug
%option nodefault
%option yyclass="MC::Scanner"
%option noyywrap
%option c++

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
%{          /** Code executed at the beginning of yylex **/
            yylval = lval;
%}

"+"			return '+';
"-"			return '-';
"*"			return '*';
"/"			return '/';
"%"			return '%';

"<"			return token::LT;
"<="		return token::LE;
">"			return token::GT;
">="		return token::GE;
"=="		return token::EQ;
"!="		return token::NE;

"true"		return token::TRUE;
"false"		return token::FALSE;
"||"		return token::OR;
"&&"		return token::AND;
"!"			return token::NEG;

"while"		return token::WHILE;
"if"		return token::IF;
"else"		return token::ELSE;
"for"       return token::FOR;

"new"       return token::NEW;
"object"    return token::OBJECT;

"int"       return token::INT;
"double"    return token::DOUBLE;
"bool"      return token::BOOL;
"string"    return token::STRING;

"return"	return token::RETURN;

"print"		return token::PRINT;
"println"   return token::PRINTLN;

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
//	yylval.intVal = val;
	yylval->build(val);
	return token::INT_VALUE;
}

{double} {
	double val = atof(yytext);
//	yylval.doubleVal = val;
	yylval->build(val);
	return token::DOUBLE_VALUE;
}

{string} {
//	char* text = (char*) malloc(sizeof(char) * (yyleng - 1));
//	// strip leading and trailing quotation marks
//	strncpy(text, yytext+1, yyleng - 1);
//	text[yyleng-2] = '\0';
//	yylval.stringVal = text;

	yylval->build<std::string>(yytext);
    return token::STRING_VALUE;
}

{identifier} {
	if (yyleng > 16) {
		yyleng = 16;
		yytext[16] = 0;
	}

//	char* id = (char*) malloc(sizeof(char) * (yyleng + 1));
//	strncpy(id, yytext, yyleng + 1);
//	yylval.identifier = id;
	yylval->build<std::string>(yytext);
	return token::IDENTIFIER;
}

"\n" {
	// Update line number
	// loc->lines();
	// return token::NEWLINE;
}

{whitespace} {
}

. {
	return token::LEX_ERROR;
}

%%


