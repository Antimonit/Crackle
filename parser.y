%{

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include "types.h"
#include "interpreter.h"
#include "symbol_table.h"
#include "parser.h"

int yylex(void);
int yyerror(const char* message);
int lineCounter;

node* empty();
node* funCall(const char* value, node* params);
node* constantInt(int value);
node* constantDouble(double value);
node* constantString(char* value);
node* constantBool(bool value);
node* variable(const char* value);
node* op(int oper, int opCount, ...);
node* ex(node* p);
void freeNode(node* p);

%}

%union {
	int intVal;
	double doubleVal;
	char* stringVal;
	bool boolVal;
	char* identifier;
	node* node;
	nodeTypeEnum type;
}

%token LEX_ERROR

%token <intVal> INT_VALUE
%token <doubleVal> DOUBLE_VALUE
%token <stringVal> STRING_VALUE
%token <boolVal> TRUE
%token <boolVal> FALSE
%token <identifier> IDENTIFIER

%token VAR FUN RETURN
%token PRINT
%nonassoc LT LE GT GE EQ NE
%left OR
%left AND
%right NEG
%right ASSIGN
%left '+' '-'
%left '*' '/' '%'
%nonassoc UMINUS
%token '(' ')'
%token '{' '}'
%token ';' ','
%token IF ELSE WHILE DO FOR
%token INT DOUBLE BOOL STRING

%type <node> primitive_value expression lex_error
%type <node> statement statement_list
%type <node> if_statement while_statement
%type <node> var_declaration
%type <node> fun_definition fun_param_list
%type <node> func_call argument_expression_list
%type <node> return_statement
%type <type> type_specifier

%%

program
		: statement {
			node* result = ex($1);
//			freeNode(result);
//			freeNode($1);
		} program
		|

statement
		: PRINT statement		{ $$ = op(PRINT, 1, $2); }
		| expression ';'		{ $$ = $1; }
		| if_statement			{ $$ = $1; }
		| while_statement		{ $$ = $1; }
		| var_declaration		{ $$ = $1; }
		| fun_definition		{ $$ = $1; }
		| return_statement		{ $$ = $1; }
		| error					{ printf("\n"); }

statement_list
		: statement_list statement	{ $$ = op(';', 2, $1, $2); }
		| { $$ = op(';', 0); }

if_statement
		: IF '(' expression ')' '{' statement_list '}'                                { $$ = op(IF, 2, $3, $6); }
		| IF '(' expression ')' '{' statement_list '}' ELSE '{' statement_list '}'    { $$ = op(IF, 3, $3, $6, $10); }

while_statement
		: WHILE '(' expression ')' '{' statement_list '}' 	{ $$ = op(WHILE, 2, $3, $6); }

type_specifier
		: INT		{ $$ = typeInt; }
		| DOUBLE	{ $$ = typeDouble; }
		| BOOL	    { $$ = typeBool; }
		| STRING	{ $$ = typeString; }

var_declaration
		: type_specifier IDENTIFIER ';' { $$ = op(VAR, 1, typedVariable($2, $1)); }

fun_definition
		: type_specifier IDENTIFIER '(' fun_param_list ')' '{' statement_list '}' {
			$$ = op(FUN, 3, typedVariable($2, $1), $4, $7);
		}
		| type_specifier IDENTIFIER '(' ')' '{' statement_list '}' {
			$$ = op(FUN, 3, typedVariable($2, $1), NULL, $6);
		}

fun_param_list
		: type_specifier IDENTIFIER							{ $$ = op(',', 2, typedVariable($2, $1), NULL); }
		| fun_param_list ',' type_specifier IDENTIFIER		{ $$ = op(',', 2, $1, typedVariable($4, $3)); }

return_statement
		: RETURN expression ';'			{ $$ = op(RETURN, 1, $2); }

func_call
		: IDENTIFIER '(' argument_expression_list ')'	{ $$ = funCall($1, $3); }
		| IDENTIFIER '(' ')' 							{ $$ = funCall($1, NULL); }

argument_expression_list
		: expression								{ $$ = $1; }
		| argument_expression_list ',' expression	{ $$ = op(',', 2, $1, $3); }

expression
		: '(' expression ')'			{ $$ = $2; }
		| expression OR expression		{ $$ = op(OR, 2, $1, $3); }
		| expression AND expression		{ $$ = op(AND, 2, $1, $3); }
		| NEG expression				{ $$ = op(NEG, 1, $2); }
		| expression LT expression		{ $$ = op(LT, 2, $1, $3); }
		| expression LE expression		{ $$ = op(LE, 2, $1, $3); }
		| expression GT expression		{ $$ = op(GT, 2, $1, $3); }
		| expression GE expression		{ $$ = op(GE, 2, $1, $3); }
		| expression EQ expression		{ $$ = op(EQ, 2, $1, $3); }
		| expression NE expression		{ $$ = op(NE, 2, $1, $3); }
		| expression '+' expression		{ $$ = op('+', 2, $1, $3); }
		| expression '-' expression		{ $$ = op('-', 2, $1, $3); }
		| expression '*' expression		{ $$ = op('*', 2, $1, $3); }
		| expression '/' expression		{ $$ = op('/', 2, $1, $3); }
		| expression '%' expression		{ $$ = op('%', 2, $1, $3); }
		| '-' expression %prec UMINUS	{ $$ = op(UMINUS, 1, $2); }
		| IDENTIFIER ASSIGN expression	{ $$ = op(ASSIGN, 2, variable($1), $3); }
		| IDENTIFIER                    { $$ = variable($1); }
		| func_call			{ $$ = $1; }
		| primitive_value	{ $$ = $1; }
		| lex_error			{ $$ = $1; }

primitive_value
		: INT_VALUE		{ $$ = constantInt($1); }
		| DOUBLE_VALUE	{ $$ = constantDouble($1); }
		| STRING_VALUE	{ $$ = constantString($1); }
		| TRUE			{ $$ = constantBool(true); }
		| FALSE			{ $$ = constantBool(false); }

lex_error
		: LEX_ERROR {
			$$ = empty();
			yyerror("lexical error");
			YYERROR;
		}

%%


int main(void) {
	yyparse();
	return 0;
}

int yywrap() {
	return 1;
}
