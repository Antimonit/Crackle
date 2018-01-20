%{
#include <string>
#include "scanner.hpp"

int yyerror(std::string message);

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

"||"		return token::OR;
"&&"		return token::AND;
"!"			return token::NEG;

"while"		return token::WHILE;
"if"		return token::IF;
"else"		return token::ELSE;
"for"       return token::FOR;

"new"       return token::NEW;
"null"      return token::NULL_VALUE;
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
// TODO ignore invalid integer
//	yyerror("bad integer");
}

"true" {
	yylval->build(true);
	return token::BOOL_VALUE;
}

"false" {
	yylval->build(false);
	return token::BOOL_VALUE;
}

{integer} {
	if (yyleng > 10) {
		yyleng = 10;
		yytext[10] = 0;
	}
	int val = atoi(yytext);
	yylval->build(val);
	return token::INT_VALUE;
}

{double} {
	double val = atof(yytext);
	yylval->build(val);
	return token::DOUBLE_VALUE;
}

{string} {
	std::string unquoted = std::string(yytext).substr(1, yyleng - 2);
	yylval->build<std::string>(unquoted);
    return token::STRING_VALUE;
}

{identifier} {
	if (yyleng > 16) {
		yyleng = 16;
		yytext[16] = 0;
	}
	yylval->build<std::string>(yytext);
	return token::IDENTIFIER;
}

"\n" {
	loc->lines();
	// return token::NEWLINE;
}

{whitespace} {
}

. {
	return token::LEX_ERROR;
}

%%


