%{

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "types.h"
#include "interpreter.h"
#include "symbols.h"

int yyerror(const char* message);
int yylex(void);

node* empty();
node* fun(const char* value, node* params);
node* constantInt(int value);
node* constantDouble(double value);
node* variable(const char* value);
node* op(int oper, int opCount, ...);
node* ex(node* p);
void freeNode(node* p);

char temp[100];

int freshNewline;

%}
 
%union {
	int intVal;
	double doubleVal;
	char* identifier;
    node* node;
}


%token LEX_ERROR

%token <intVal> INTEGER_VALUE
%token <doubleVal> DOUBLE_VALUE
%token <identifier> IDENTIFIER

%token DEF LOCAL RETURN
%nonassoc LT LE GT GE EQ NE
%left OR
%left AND
%right NEG
%right ASSIGN
%token TRUE FALSE
%left PLUS MINUS
%left MULTIPLY DIVIDE
%nonassoc UMINUS
%token LPAREN RPAREN
%token DELIMITER COMMA
%token IF ELSE WHILE END

%type <node> value expression lex_error
%type <node> statement statement_list
%type <node> if_statement while_statement
%type <node> func_definition func_param_list
%type <node> func_call argument_expression_list
%type <node> return_statement local_statement
%type <node> bool comparison_operand

%%

program
		: statement {
			node* resultP = ex($1);
			node result = *resultP;
			//freeNode($1);
			
			switch (result.type) {
				case typeOperator:
					switch (result.oper.oper) {
						case WHILE:
							printf("while\n");
							break;
						case IF:
							printf("if\n");
							break;
						case DEF:
							printf("Function defined\n");
							break;
						default:
							printf("WRONG OPERATOR\n");
					}
					break;
				case typeConstant:
					switch (result.constant.type) {
						case typeInt:
							printf("%d\n", result.constant.intVal);
							break;
						case typeDouble:
							printf("%f\n", result.constant.doubleVal);
							break;
					}
					break;
				case typeFunctionDef:
					printf("%s defined\n", result.function.name);
					break;
				case typeEmpty:
					break;
				default:
					printf("WRONG TYPE\n");
			}
			
			freshNewline = 1;
		} program
		|

statement
		: expression DELIMITER	{ $$ = $1; }
	 	| if_statement			{ $$ = $1; }
	 	| while_statement		{ $$ = $1; }
	 	| func_definition		{ $$ = $1; }
	 	| return_statement		{ $$ = $1; }
	 	| local_statement		{ $$ = $1; }
		| error					{ printf("\n"); }

statement_list
		: statement					{ $$ = $1; }
        | statement_list statement	{ $$ = op(DELIMITER, 2, $1, $2); }

if_statement
		: IF LPAREN bool RPAREN statement_list END						{ $$ = op(IF, 2, $3, $5); }
		| IF LPAREN bool RPAREN statement_list ELSE statement_list END	{ $$ = op(IF, 3, $3, $5, $7); }

while_statement
		: WHILE LPAREN bool RPAREN statement_list END 	{ $$ = op(WHILE, 2, $3, $5); }

bool
		: bool OR bool			{ $$ = op(OR, 2, $1, $3); }
		| bool AND bool			{ $$ = op(AND, 2, $1, $3); }
		| NEG bool				{ $$ = op(NEG, 1, $2); }
		| LPAREN bool RPAREN	{ $$ = $2; }
		| expression comparison_operand expression { $$ = op($2->oper.oper, 2, $1, $3); }
		| TRUE					{ $$ = constantInt(1); }
		| FALSE					{ $$ = constantInt(0); }

comparison_operand
		: LT { $$ = op(LT, 0); }
		| LE { $$ = op(LE, 0); }
		| GT { $$ = op(GT, 0); }
		| GE { $$ = op(GE, 0); }
		| EQ { $$ = op(EQ, 0); }
		| NE { $$ = op(NE, 0); }

func_definition
		: DEF IDENTIFIER LPAREN func_param_list RPAREN statement_list END	{ $$ = op(DEF, 3, variable($2), $4, $6); }
		| DEF IDENTIFIER LPAREN RPAREN statement_list END					{ $$ = op(DEF, 3, variable($2), NULL, $5); }

func_param_list
		: IDENTIFIER							{ $$ = variable($1); }
		| func_param_list COMMA IDENTIFIER		{ $$ = op(COMMA, 2, $1, variable($3)); }

return_statement
		: RETURN expression DELIMITER			{ $$ = op(RETURN, 1, $2); }

local_statement
		: LOCAL IDENTIFIER DELIMITER			{ $$ = op(LOCAL, 0); }

func_call
		: IDENTIFIER LPAREN argument_expression_list RPAREN	{ $$ = fun($1, $3); }
		| IDENTIFIER LPAREN RPAREN 							{ $$ = fun($1, NULL); }
		
argument_expression_list
		: expression								{ $$ = $1; }
		| argument_expression_list COMMA expression	{ $$ = op(COMMA, 2, $1, $3); }

expression
		: expression PLUS     expression	{ $$ = op(PLUS    , 2, $1, $3); }
		| expression MINUS    expression	{ $$ = op(MINUS   , 2, $1, $3); }
		| expression MULTIPLY expression	{ $$ = op(MULTIPLY, 2, $1, $3); }
		| expression DIVIDE   expression	{ $$ = op(DIVIDE  , 2, $1, $3); }
		| MINUS expression %prec UMINUS		{ $$ = op(UMINUS  , 1, $2); }
		| LPAREN expression RPAREN			{ $$ = $2; }
		| IDENTIFIER ASSIGN expression		{ $$ = op(ASSIGN  , 2, variable($1), $3); }
		| IDENTIFIER {
			int index = findSymbol($1);
			if (index == -1) {
				$$ = empty();
				sprintf(temp, "%s is undefined", $1);
				yyerror(temp);
				YYERROR;
			} else {
				$$ = variable($1);
			}
		}
	 	| func_call		{ $$ = $1; }
		| value			{ $$ = $1; }
		| lex_error		{ $$ = $1; }

value
		: INTEGER_VALUE	{ $$ = constantInt($1); }
		| DOUBLE_VALUE	{ $$ = constantDouble($1); }

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

node* fun(const char* value, node* params) {
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
	printf("?- ");
	yyparse();
	return 0;
}

int yywrap() {
	return 1;
}

int yyerror(const char* message) {
	printf(message);
}
