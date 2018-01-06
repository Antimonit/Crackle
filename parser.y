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
node* type(int type);
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
}

%token LEX_ERROR

%token <intVal> INT_VALUE
%token <doubleVal> DOUBLE_VALUE
%token <stringVal> STRING_VALUE
%token <boolVal> TRUE
%token <boolVal> FALSE
%token <identifier> IDENTIFIER

%token VAR FUN RETURN
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

%token PRINT

%type <node> primitive_value expression lex_error
%type <node> statement statement_list
%type <node> if_statement while_statement
%type <node> var_declaration
%type <node> fun_definition fun_param_list
%type <node> func_call argument_expression_list
%type <node> return_statement
%type <node> bool comparison_operand
%type <node> type_specifier

%%

program
		: statement {
			node* resultP = ex($1);
			node result = *resultP;
			//freeNode($1);

			switch (result.type) {
				case typeOperator:
					switch (result.oper.oper) {
						case WHILE: break;
						case IF:	break;
						case FUN:	break;
						case VAR:	break;
						default:	printf("WRONG OPERATOR\n");
					}
					break;
				case typeConstant:
					printf("%s\n", getConstantValueString(result.constant));
					break;
				case typeFunctionDef:
					printf("Function %s defined\n", result.function.name);
					break;
				case typeEmpty:
					break;
				default:
					printf("WRONG TYPE\n");
			}
		} program
		|

statement
		: expression ';'		{ $$ = $1; }
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
		: IF '(' bool ')' '{' statement_list '}' {
			 $$ = op(IF, 2, $3, $6);
		}
		| IF '(' bool ')' '{' statement_list '}' ELSE '{' statement_list '}' {
			 $$ = op(IF, 3, $3, $6, $10);
		}

while_statement
		: WHILE '(' bool ')' '{' statement_list '}' 	{ $$ = op(WHILE, 2, $3, $6); }

bool
		: bool OR bool		{ $$ = op(OR, 2, $1, $3); }
		| bool AND bool		{ $$ = op(AND, 2, $1, $3); }
		| NEG bool			{ $$ = op(NEG, 1, $2); }
		| '(' bool ')'		 { $$ = $2; }
		| expression comparison_operand expression { $$ = op($2->oper.oper, 2, $1, $3); }
		| TRUE				{ $$ = constantBool(true); }
		| FALSE				{ $$ = constantBool(false); }

comparison_operand
		: LT { $$ = op(LT, 0); }
		| LE { $$ = op(LE, 0); }
		| GT { $$ = op(GT, 0); }
		| GE { $$ = op(GE, 0); }
		| EQ { $$ = op(EQ, 0); }
		| NE { $$ = op(NE, 0); }

type_specifier
		: INT		{ $$ = type(INT); }
		| DOUBLE	{ $$ = type(DOUBLE); }
		| BOOL	    { $$ = type(BOOL); }
		| STRING	{ $$ = type(STRING); }

var_declaration
		: VAR IDENTIFIER ':' type_specifier ';' { $$ = op(VAR, 1, typedVariable($2, $4->dataType.type)); }

fun_definition
		: FUN IDENTIFIER '(' fun_param_list ')' ':' type_specifier '{' statement_list '}' {
			$$ = op(FUN, 3, typedVariable($2, $7->dataType.type), $4, $9);
		}
		| FUN IDENTIFIER '(' ')' ':' type_specifier '{' statement_list '}' {
			$$ = op(FUN, 3, typedVariable($2, $6->dataType.type), NULL, $8);
		}

fun_param_list
		: IDENTIFIER ':' type_specifier							{ $$ = op(',', 2, typedVariable($1, $3->dataType.type), NULL); }
		| fun_param_list ',' IDENTIFIER ':' type_specifier		{ $$ = op(',', 2, $1, typedVariable($3, $5->dataType.type)); }

return_statement
		: RETURN expression ';'			{ $$ = op(RETURN, 1, $2); }

func_call
		: IDENTIFIER '(' argument_expression_list ')'	{ $$ = funCall($1, $3); }
		| IDENTIFIER '(' ')' 							{ $$ = funCall($1, NULL); }

argument_expression_list
		: expression								{ $$ = $1; }
		| argument_expression_list ',' expression	{ $$ = op(',', 2, $1, $3); }

expression
		: expression '+' expression	{ $$ = op('+', 2, $1, $3); }
		| expression '-' expression	{ $$ = op('-', 2, $1, $3); }
		| expression '*' expression	{ $$ = op('*', 2, $1, $3); }
		| expression '/' expression	{ $$ = op('/', 2, $1, $3); }
		| expression '%' expression	{ $$ = op('%', 2, $1, $3); }
		| '-' expression %prec UMINUS	{ $$ = op(UMINUS, 1, $2); }
		| '(' expression ')'			{ $$ = $2; }
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
