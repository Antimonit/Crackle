%skeleton "lalr1.cc"
%require  "3.0"
%defines
%define api.namespace {MC}
%define parser_class_name {Parser}
%define parse.trace
%error-verbose

%code requires {
	namespace MC {
		class Driver;
        class Scanner;
	}
	#include "headers/types.hpp"

	/* The following definitions is missing when %locations isn't used */
	#ifndef YY_NULLPTR
	 #if defined __cplusplus && 201103L <= __cplusplus
	  #define YY_NULLPTR nullptr
	 #else
	  #define YY_NULLPTR 0
	 #endif
	#endif
}

%parse-param { Scanner &scanner }
%parse-param { Driver &driver }

%define api.value.type variant
%define parse.assert

%code {
	#include <iostream>
	#include <cstdlib>
	#include <fstream>
	#include "driver.hpp"
	#include "headers/types.hpp"
	#include "parser.hpp"

	#undef yylex
	#define yylex scanner.yylex
}

%token <int> INT_VALUE "integer"
%token <double> DOUBLE_VALUE "double"
%token <std::string> STRING_VALUE "string"
%token <bool> BOOL_VALUE "bool"
%token <void*> NULL_VALUE "null"
%token <std::string> IDENTIFIER "identifier"
%token <Node*> NODE "node"
%token <DataType> DATA_TYPE_ENUM "dataType"

%token NEW OBJECT
%token VAR FUN RETURN
%token PRINT PRINTLN
%nonassoc LT LE GT GE EQ NE
%nonassoc INC DEC
%left OR
%left AND
%right NEG
%right '='
%left '+' '-'
%left '*' '/' '%'
%nonassoc UMINUS
%token '(' ')'
%token '{' '}'
%token ';'
%token ','
%nonassoc '.'
%token IF ELSE WHILE DO FOR
%token INT DOUBLE BOOL STRING
%token LEX_ERROR

%type <Node*> primitive_value object_value variable_value
%type <Node*> expression lex_error
%type <Node*> statement statement_list
%type <Node*> expression_statement for_statement if_statement while_statement
%type <Node*> var_definition var_definition_list
%type <Node*> fun_definition fun_param_list
%type <Node*> fun_call argument_expression_list
%type <Node*> return_statement
%type <Node*> typed_identifier
%type <Node*> object_definition
%type <DataType> type_specifier

%locations

%start program
%%

program
		: statement {
			driver.ex($1);
//			Node* result = ex($1);
//			freeNode(result);
//			deleteNode($1);
		} program
		|

statement
		: PRINT statement		{ $$ = op(token::PRINT, 1, $2); }
		| PRINTLN statement     { $$ = op(token::PRINTLN, 1, $2); }
		| expression_statement	{ $$ = $1; }
		| if_statement			{ $$ = $1; }
		| while_statement		{ $$ = $1; }
		| for_statement         { $$ = $1; }
		| object_definition     { $$ = $1; }
		| var_definition		{ $$ = $1; }
		| fun_definition		{ $$ = $1; }
		| return_statement		{ $$ = $1; }
		| error					{ printf("\n"); }

statement_list
		: statement_list statement	{ $$ = op(';', 2, $1, $2); }
		| { $$ = op(';', 0); }

expression_statement
		: ';'                   { $$ = op(';', 0); }
		| expression ';'        { $$ = $1; }

if_statement
		: IF '(' expression ')' '{' statement_list '}'                                { $$ = op(token::IF, 2, $3, $6); }
		| IF '(' expression ')' '{' statement_list '}' ELSE '{' statement_list '}'    { $$ = op(token::IF, 3, $3, $6, $10); }

while_statement
		: WHILE '(' expression ')' '{' statement_list '}' 	{ $$ = op(token::WHILE, 2, $3, $6); }

for_statement
		: FOR '(' expression_statement expression_statement ')' '{' statement_list '}'  {
			$$ = op(token::FOR, 4, $3, $4, NULL, $7);
		}
		| FOR '(' expression_statement expression_statement expression ')' '{' statement_list '}' {
			$$ = op(token::FOR, 4, $3, $4, $5, $8);
		}

typed_identifier
		: type_specifier IDENTIFIER { $$ = typedVariable($2, $1); }

type_specifier
		: INT		{ $$ = typeInt; }
		| DOUBLE	{ $$ = typeDouble; }
		| BOOL	    { $$ = typeBool; }
		| STRING	{ $$ = typeString; }
		| IDENTIFIER{ $$ = typeObject; }

var_definition
		: typed_identifier ';' { $$ = variableDef($1, NULL); }
		| typed_identifier '=' expression ';' { $$ = variableDef($1, $3); }

fun_definition
		: typed_identifier '(' fun_param_list ')' '{' statement_list '}' {
			$$ = functionDef($1, $3, $6);
		}
		| typed_identifier '(' ')' '{' statement_list '}' {
			$$ = functionDef($1, NULL, $5);
		}

var_definition_list
		: var_definition                        { $$ = op(',', 2, NULL, $1); }
		| var_definition_list var_definition	{ $$ = op(',', 2, $1, $2); }

object_definition
		: OBJECT IDENTIFIER '{' var_definition_list '}' { $$ = objectDef($2, $4); }

fun_param_list
		: typed_identifier								{ $$ = op(',', 2, NULL, $1); }
		| fun_param_list ',' typed_identifier			{ $$ = op(',', 2, $1, $3); }

return_statement
		: RETURN expression ';'							{ $$ = op(token::RETURN, 1, $2); }

fun_call
		: IDENTIFIER '(' argument_expression_list ')'	{ $$ = function($1, $3); }
		| IDENTIFIER '(' ')' 							{ $$ = function($1, NULL); }

argument_expression_list
		: expression									{ $$ = op(',', 2, NULL, $1); }
		| argument_expression_list ',' expression		{ $$ = op(',', 2, $1, $3); }

expression
		: '(' expression ')'			{ $$ = $2; }
		| NEG expression				{ $$ = op(token::NEG, 1, $2); }
		| expression OR expression		{ $$ = op(token::OR, 2, $1, $3); }
		| expression AND expression		{ $$ = op(token::AND, 2, $1, $3); }
		| expression LT expression		{ $$ = op(token::LT, 2, $1, $3); }
		| expression LE expression		{ $$ = op(token::LE, 2, $1, $3); }
		| expression GT expression		{ $$ = op(token::GT, 2, $1, $3); }
		| expression GE expression		{ $$ = op(token::GE, 2, $1, $3); }
		| expression EQ expression		{ $$ = op(token::EQ, 2, $1, $3); }
		| expression NE expression		{ $$ = op(token::NE, 2, $1, $3); }
		| expression '+' expression		{ $$ = op('+', 2, $1, $3); }
		| expression '-' expression		{ $$ = op('-', 2, $1, $3); }
		| expression '*' expression		{ $$ = op('*', 2, $1, $3); }
		| expression '/' expression		{ $$ = op('/', 2, $1, $3); }
		| expression '%' expression		{ $$ = op('%', 2, $1, $3); }
		| '-' expression %prec UMINUS	{ $$ = op(token::UMINUS, 1, $2); }
		| variable_value '=' expression { $$ = op('=', 2, $1, $3); }
		| fun_call						{ $$ = $1; }
		| primitive_value				{ $$ = $1; }
		| object_value                  { $$ = $1; }
		| variable_value                { $$ = $1; }
		| lex_error						{ $$ = $1; }

primitive_value
		: INT_VALUE		{ $$ = constantInt($1); }
		| DOUBLE_VALUE	{ $$ = constantDouble($1); }
		| STRING_VALUE	{ $$ = constantString($1); }
		| BOOL_VALUE    { $$ = constantBool($1); }

object_value
		: NEW IDENTIFIER    { $$ = object($2); }
		| NULL_VALUE        { $$ = constantNull(); }

variable_value
		: IDENTIFIER                    { $$ = variable($1); }
		| expression '.' IDENTIFIER     { $$ = op('.', 2, $1, variable($3)); }

lex_error
		: LEX_ERROR {
			$$ = empty();
			yyerror("lexical error");
			YYERROR;
		}

%%


void MC::Parser::error(const location_type &l, const std::string& err_message) {
	std::cerr << "Error: " << err_message << " at " << l << "\n";
}
