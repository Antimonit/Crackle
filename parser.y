%{

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include "types.h"
#include "interpreter.h"
#include "symbol_table.h"

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

char temp[100];

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
%left '*' '/'
%nonassoc UMINUS
%token '(' ')'
%token '{' '}'
%token ';' ','
%token IF ELSE WHILE
%token INT_TYPE DOUBLE_TYPE BOOL_TYPE STRING_TYPE

%token PRINT

%type <node> primitive_value expression lex_error
%type <node> statement statement_list
%type <node> if_statement while_statement
%type <node> var_definition
%type <node> fun_definition func_param_list
%type <node> func_call argument_expression_list
%type <node> return_statement
%type <node> bool comparison_operand
%type <node> data_type

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
						case IF:    break;
						case FUN:   break;
						case VAR:   break;
						default:    printf("WRONG OPERATOR\n");
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
		: expression ';'	    { $$ = $1; }
	 	| if_statement			{ $$ = $1; }
	 	| while_statement		{ $$ = $1; }
	 	| var_definition        { $$ = $1; }
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
		| '(' bool ')'	    { $$ = $2; }
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

data_type
        : INT_TYPE      { $$ = op(INT_TYPE, 0); }
        | DOUBLE_TYPE   { $$ = op(DOUBLE_TYPE, 0); }
        | BOOL_TYPE     { $$ = op(BOOL_TYPE, 0); }
        | STRING_TYPE   { $$ = op(STRING_TYPE, 0); }

var_definition
        : VAR IDENTIFIER ':' data_type ';' { $$ = op(VAR, 2, variable($2), $4); }

fun_definition
		: FUN IDENTIFIER '(' func_param_list ')' ':' data_type '{' statement_list '}' {
            $$ = op(FUN, 4, variable($2), $4, $7, $9);
		}
		| FUN IDENTIFIER '(' ')' ':' data_type '{' statement_list '}' {
		$$ = op(FUN, 4, variable($2), NULL, $6, $8);
		}

func_param_list
		: IDENTIFIER							{ $$ = variable($1); }
		| func_param_list ',' IDENTIFIER		{ $$ = op(',', 2, $1, variable($3)); }

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
		| '-' expression %prec UMINUS   { $$ = op(UMINUS, 1, $2); }
		| '(' expression ')'			{ $$ = $2; }
		| IDENTIFIER ASSIGN expression	{ $$ = op(ASSIGN, 2, variable($1), $3); }
		| IDENTIFIER {
			constantNode* symbol = findSymbolNode($1);
			if (symbol == NULL) {
				$$ = empty();
				sprintf(temp, "%s is undefined", $1);
				yyerror(temp);
				YYERROR;
			} else {
				$$ = variable($1);
			}
		}
	 	| func_call			{ $$ = $1; }
		| primitive_value	{ $$ = $1; }
		| lex_error			{ $$ = $1; }

primitive_value
		: INT_VALUE		{ $$ = constantInt($1); }
		| DOUBLE_VALUE	{ $$ = constantDouble($1); }
		| STRING_VALUE	{ $$ = constantString($1); }
		| TRUE          { $$ = constantBool(true); }
		| FALSE         { $$ = constantBool(false); }

lex_error
		: LEX_ERROR {
			$$ = empty();
			yyerror("lexical error");
			YYERROR;
		}

%%

node* newNode() {
	node* p = (node*) malloc(sizeof(node));

	if (p == NULL)
		yyerror("out of memory");

	return p;
}

node* empty() {
	node* node = newNode();

	node->type = typeEmpty;

	return node;
}

node* funCall(const char* value, node* params) {
	node* node = newNode();

	node->type = typeFunctionCall;
	node->function.name = value;

	return node;
}

node* constantInt(int value) {
	node* node = newNode();

	node->type = typeConstant;
	node->constant.type = typeInt;
	node->constant.intVal = value;

	return node;
}

node* constantDouble(double value) {
	node* node = newNode();

	node->type = typeConstant;
	node->constant.type = typeDouble;
	node->constant.doubleVal = value;

	return node;
}

node* constantString(char* value) {
	node* node = newNode();

	node->type = typeConstant;
	node->constant.type = typeString;
	node->constant.stringVal = value;

	return node;
}

node* constantBool(bool value) {
	node* node = newNode();

	node->type = typeConstant;
	node->constant.type = typeBool;
	node->constant.boolVal = value;

	return node;
}

node* variable(const char* value) {
	node* node = newNode();

	node->type = typeVariable;
	node->variable.name = value;

	return node;
}

node* op(int oper, int opCount, ...) {
	/* allocate node, extending op array */
    node* p = malloc(sizeof(node) + (opCount-1) * sizeof(node*));

    if (p == NULL)
        yyerror("out of memory");

    p->type = typeOperator;
    p->oper.oper = oper;
    p->oper.opCount = opCount;

    va_list ap;
    va_start(ap, opCount);
    for (int i = 0; i < opCount; i++) {
        p->oper.op[i] = va_arg(ap, node*);
	}
    va_end(ap);

    return p;
}

void freeNode(node* node) {
    if (!node)
    	return;

    if (node->type == typeOperator) {
        for (int i = 0; i < node->oper.opCount; i++) {
            freeNode(node->oper.op[i]);
		}
    }
    free(node);
}

int main(void) {
	yyparse();
	return 0;
}

int yywrap() {
	return 1;
}

int yyerror(const char* message) {
	printf(message);
}
