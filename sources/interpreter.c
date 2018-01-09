#include <stdio.h>
#include <stdlib.h>
#include <headers/types.h>
#include <headers/ast_printer.h>
#include "headers/interpreter.h"
#include "headers/symbol_table.h"
#include "headers/function_symbol_table.h"
#include "headers/node_helpers.h"
#include "y.tab.h"
#include "headers/debug.h"
#include "headers/types.h"


void var(node* p, node* result) {
	node* varNode = p->oper.op[0];
	variableNode variableNode = varNode->variable;

	constantNode* variable = addSymbolNode(variableNode);
	if (variable == NULL) {
		result->type = typeEmpty;
		return;
	}

	const char* name = variableNode.name;
	dataTypeEnum varType = variable->dataType;

	result->type = typeConstant;

	if (p->oper.opCount > 1) {
		node* defaultValue = ex(p->oper.op[1]);
		dataTypeEnum exType = defaultValue->constant.dataType;
		if (varType != exType) {
			printf("Warning: Incompatible assignment of type '%s' to type '%s'\n",
				   dataTypeToString(exType), dataTypeToString(varType));
			result->type = typeEmpty;
			return;
		}

		result->constant = *variable = defaultValue->constant;
		debug("\tnode: Operand Var %s %s %s\n",
			  dataTypeToString(result->constant.dataType), name, constantValueToString(*variable));

	} else {
		result->constant.dataType = varType;
		if (varType == typeInt) {
			result->constant.intVal = variable->intVal = 0;
			debug("\tnode: Operand Var int %s %d\n", name, variable->intVal);
		} else if (varType == typeDouble) {
			result->constant.doubleVal = variable->doubleVal = 0.0;
			debug("\tnode: Operand Var double %s %f\n", name, variable->doubleVal);
		} else if (varType == typeString) {
			result->constant.stringVal = variable->stringVal = "";
			debug("\tnode: Operand Var string %s %s\n", name, variable->stringVal);
		} else if (varType == typeBool) {
			result->constant.boolVal = variable->boolVal = false;
			debug("\tnode: Operand Var bool %s %s\n", name, variable->boolVal == true ? "true" : "false");
		}
	}
}

/*private*/ void assignVal(constantNode* lval, constantNode* rval, dataTypeEnum varType) {
	if (varType == typeInt) {
		lval->intVal = rval->intVal;
	} else if (varType == typeDouble) {
		lval->doubleVal = rval->doubleVal;
	} else if (varType == typeString) {
		lval->stringVal = rval->stringVal;
	} else if (varType == typeBool) {
		lval->boolVal = rval->boolVal;
	}
}

void returnx(node* p, node* result) {
	node* value = ex(p->oper.op[0]);

	result->type = typeReturn;
	result->ret.value = value->constant;
}

void comma(node* p, node* result) {
	node* left = p->oper.op[0];
	node* right = p->oper.op[1];

	/*
	if (left->type == typeVariable) {
		node* leftConstant = ex(left);
		node* rightConstant = ex(right);
	} else {
		node* exx = ex(left);
	}
	*/
}

void whilex(node* p, node* result) {
	while (ex(p->oper.op[0])->constant.intVal) {
		pushSymbolTableScope();
		node* left = ex(p->oper.op[1]);
		if (left->type == typeReturn) {
			result->type = typeReturn;
			result->ret.value = left->ret.value;
			popSymbolTableScope();
			return;
		}
		popSymbolTableScope();
	}

	result->type = typeOperator;
	result->oper.oper = WHILE;
}

void forx(node* p, node* result) {
	for (ex(p->oper.op[0]);
		 ex(p->oper.op[1])->constant.boolVal;
		 ex(p->oper.op[2])) {

		pushSymbolTableScope();
		node* left = ex(p->oper.op[3]);
		if (left->type == typeReturn) {
			popSymbolTableScope();
			result->type = typeReturn;
			result->ret.value = left->ret.value;
			return;
		}
		popSymbolTableScope();
	}
	result->type = typeOperator;
	result->oper.oper = FOR;
}

void ifx(node* p, node* result) {
	pushSymbolTableScope();
	if (ex(p->oper.op[0])->constant.intVal) {
		node* left = ex(p->oper.op[1]);
		if (left->type == typeReturn) {
			result->type = left->type;
			result->ret.value = left->ret.value;
			popSymbolTableScope();
			return;
		}
	} else if (p->oper.opCount > 2) {
		node* right = ex(p->oper.op[2]);
		if (right->type == typeReturn) {
			result->type = right->type;
			result->ret.value = right->ret.value;
			popSymbolTableScope();
			return;
		}
	}

	result->type = typeOperator;
	result->oper.oper = IF;
	popSymbolTableScope();
}

void delimiter(node* p, node* result) {
	result->type = typeOperator;
	result->oper.oper = ';';

	if (p->oper.opCount > 0) {
		node* left = ex(p->oper.op[0]);
		if (left->type == typeReturn) {
			result->type = typeReturn;
			result->ret.value = left->ret.value;
			return;
		}
	}
	if (p->oper.opCount > 1) {
		node* right = ex(p->oper.op[1]);
		if (right->type == typeReturn) {
			result->type = typeReturn;
			result->ret.value = right->ret.value;
			return;
		}
	}
}

void assign(node* p, node* result) {
	node* one = p->oper.op[0];
	variableNode variableNode = one->variable;
	node* value = ex(p->oper.op[1]);

	constantNode* variable = findSymbolNode(variableNode.name);

	if (variable == NULL) {
		printf("Warning: Unable to assign undefined variable '%s'.\n", variableNode.name);
		result->type = typeEmpty;
		return;
	}

	if (variable->dataType != value->constant.dataType) {
		printf("Warning: Incompatible assignment of type '%s' to variable '%s' of type '%s'.\n",
			   dataTypeToString(value->constant.dataType), variableNode.name, dataTypeToString(variable->dataType));
		result->type = typeEmpty;
		return;
	}

	dataTypeEnum type = variable->dataType;

	result->constant.dataType = type;
	result->constant = *variable = value->constant;

//	debug("\tnode: Operand Assign %s %s %s\n",
//		  dataTypeToString(value->constant.dataType), name, constantValueToString(value->constant));
}

void uminus(node* p, node* result) {
	node* a = ex(p->oper.op[0]);
	result->type = typeConstant;
	result->constant.dataType = a->constant.dataType;
	if (result->constant.dataType == typeInt) {
		result->constant.intVal = -a->constant.intVal;
	} else if (result->constant.dataType == typeDouble) {
		result->constant.doubleVal = -a->constant.doubleVal;
	} else {
		printf("Warning: Invalid argument to unary minus.\n");
	}
}

void plus(node* p, node* result) {
	constantNode* left = &ex(p->oper.op[0])->constant;
	constantNode* right = &ex(p->oper.op[1])->constant;
	result->type = typeConstant;
	result->constant.dataType = widenNodes(left, right);
	if (result->constant.dataType == typeInt) {
		result->constant.intVal = left->intVal + right->intVal;
	} else if (result->constant.dataType == typeDouble) {
		result->constant.doubleVal = left->doubleVal + right->doubleVal;
	} else {
		printf("Warning: Invalid argument to plus operator.\n");
	}
}
void minus(node* p, node* result) {
	constantNode* left = &ex(p->oper.op[0])->constant;
	constantNode* right = &ex(p->oper.op[1])->constant;
	result->type = typeConstant;
	result->constant.dataType = widenNodes(left, right);
	if (result->constant.dataType == typeInt) {
		result->constant.intVal = left->intVal - right->intVal;
	} else if (result->constant.dataType == typeDouble) {
		result->constant.doubleVal = left->doubleVal - right->doubleVal;
	} else {
		printf("Warning: Invalid argument to minus operator.\n");
	}
}
void multiply(node* p, node* result) {
	constantNode* left = &ex(p->oper.op[0])->constant;
	constantNode* right = &ex(p->oper.op[1])->constant;
	result->type = typeConstant;
	result->constant.dataType = widenNodes(left, right);
	if (result->constant.dataType == typeInt) {
		result->constant.intVal = left->intVal * right->intVal;
	} else if (result->constant.dataType == typeDouble) {
		result->constant.doubleVal = left->doubleVal * right->doubleVal;
	} else {
		printf("Warning: Invalid argument to multiply operator.\n");
	}
}
void divide(node* p, node* result) {
	constantNode* left = &ex(p->oper.op[0])->constant;
	constantNode* right = &ex(p->oper.op[1])->constant;
	result->type = typeConstant;
	result->constant.dataType = widenNodes(left, right);
	if (result->constant.dataType == typeInt) {
		result->constant.intVal = left->intVal / right->intVal;
	} else if (result->constant.dataType == typeDouble) {
		result->constant.doubleVal = left->doubleVal / right->doubleVal;
	} else {
		printf("Warning: Invalid argument to divide operator.\n");
	}
}
void modulo(node* p, node* result) {
	constantNode* left = &ex(p->oper.op[0])->constant;
	constantNode* right = &ex(p->oper.op[1])->constant;
	result->type = typeConstant;
	result->constant.dataType = widenNodes(left, right);
	if (result->constant.dataType == typeInt) {
		result->constant.intVal = left->intVal % right->intVal;
	} else {
		printf("Warning: Invalid argument to modulo operator.\n");
	}
}

void andx(node* p, node* result) {
	constantNode left = ex(p->oper.op[0])->constant;
	constantNode right = ex(p->oper.op[1])->constant;
	result->type = typeConstant;
	result->constant.dataType = typeUndefined;

	if (left.dataType != typeBool) {
		printf("Warning: Invalid argument of type %s to AND operator.\n", dataTypeToString(left.dataType));
	}
	if (right.dataType != typeBool) {
		printf("Warning: Invalid argument of type %s to AND operator.\n", dataTypeToString(right.dataType));
	}
	if (left.dataType == typeBool && right.dataType == typeBool) {
		result->constant.dataType = typeBool;
		result->constant.boolVal = ((left.boolVal == true) && (right.boolVal == true)) ? true : false;
	}
}
void orx(node* p, node* result) {
	constantNode left = ex(p->oper.op[0])->constant;
	constantNode right = ex(p->oper.op[1])->constant;
	result->type = typeConstant;
	result->constant.dataType = typeUndefined;

	if (left.dataType != typeBool) {
		printf("Warning: Invalid argument of type %s to OR operator.\n", dataTypeToString(left.dataType));
	}
	if (right.dataType != typeBool) {
		printf("Warning: Invalid argument of type %s to OR operator.\n", dataTypeToString(right.dataType));
	}
	if (left.dataType == typeBool && right.dataType == typeBool) {
		result->constant.dataType = typeBool;
		result->constant.boolVal = ((left.boolVal == true) || (right.boolVal == true)) ? true : false;
	}
}
void neg(node* p, node* result) {
	constantNode value = ex(p->oper.op[0])->constant;
	result->type = typeConstant;
	result->constant.dataType = typeUndefined;

	if (value.dataType != typeBool) {
		printf("Warning: Invalid argument of type %s to NEG operator.\n", dataTypeToString(value.dataType));
	}
	if (value.dataType == typeBool) {
		result->constant.dataType = typeBool;
		result->constant.boolVal = (value.boolVal) == true ? false : true;
	}
}

void lt(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = typeBool;
	result->constant.boolVal = (left->constant.intVal < right->constant.intVal) ? true : false;
}
void le(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = typeBool;
	result->constant.boolVal = (left->constant.intVal <= right->constant.intVal) ? true : false;
}
void gt(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = typeBool;
	result->constant.boolVal = (left->constant.intVal > right->constant.intVal) ? true : false;
}
void ge(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = typeBool;
	result->constant.boolVal = (left->constant.intVal >= right->constant.intVal) ? true : false;
}
void eq(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = typeBool;
	result->constant.boolVal = (left->constant.intVal == right->constant.intVal) ? true : false;
}
void ne(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = typeBool;
	result->constant.boolVal = (left->constant.intVal != right->constant.intVal) ? true : false;
}

void print(node* p, node* result) {
	node* value = ex(p->oper.op[0]);

	switch (value->type) {
		case typeOperator:
			switch (value->oper.oper) {
				case FOR:	printf("FOR");		break;
				case WHILE:	printf("WHILE");	break;
				case IF:	printf("IF");		break;
				case FUN:	printf("FUN");		break;
				case VAR:	printf("VAR");		break;
				default:	printf("WRONG OPERATOR");
			}
			break;
		case typeConstant:
			printf("%s", constantValueToString(value->constant));
			break;
		case typeFunctionDef:
			printf("Function %s defined", value->function.name);
			break;
		case typeEmpty:
			printf("EMPTY");
			break;
		default:
			printf("WRONG TYPE");
	}

	result->type = typeEmpty;
}
void println(node* p, node* result) {
	print(p, result);
	printf("\n");
}

node* ex(node* p) {
	node* result = malloc(sizeof(node));
	result->type = typeEmpty;

	if (!p)
		return result;
	
	enterNode(p);

	switch (p->type) {
		case typeConstant:
			result->type = typeConstant;
			result->constant = p->constant;
//			debug("\tnode: Constant %s %s\n", dataTypeToString(p->constant.dataType), constantValueToString(p->constant));
			break;

		case typeVariable:
			result->type = typeConstant;

			const char* name = p->variable.name;
			constantNode* variable = findSymbolNode(name);

			if (variable == NULL) {
				printf("Warning: Undefined variable '%s'.\n", name);
				result->type = typeEmpty;
				exitNode(result);
				return result;
			}

			result->constant = *variable;

//			debug("\tnode: Identifier %s %s\n", name, constantValueToString(*variable));
			break;

		case typeOperator:
			switch (p->oper.oper) {
				case PRINT:		print(p, result); break;
				case PRINTLN:	println(p, result); break;
				case VAR:		var(p, result); break;
				case RETURN:	returnx(p, result); break;
				case ',':		comma(p, result); break;

				case WHILE:		whilex(p, result); break;
				case IF:		ifx(p, result); break;
				case FOR:		forx(p, result); break;
				case ';':		delimiter(p, result); break;
				case '=':		assign(p, result); break;

				case UMINUS:	uminus(p, result); break;

				case '+':		plus(p, result); break;
				case '-':		minus(p, result); break;
				case '*':		multiply(p, result); break;
				case '/':		divide(p, result); break;
				case '%':		modulo(p, result); break;

				case AND:		andx(p, result); break;
				case OR:		orx(p, result); break;
				case NEG:		neg(p, result); break;

				case LT:		lt(p, result); break;
				case LE:		le(p, result); break;
				case GT:		gt(p, result); break;
				case GE:		ge(p, result); break;
				case EQ:		eq(p, result); break;
				case NE:		ne(p, result); break;
				default:		printf("Error: unknown operator %d", p->oper.oper);
			}
			break;

		case typeFunctionCall:
			name = p->functionCall.name;

			functionNode* function = findFunction(name);
			if (function == NULL) {
				printf("Warning: Undefined function '%s'.\n", name);
				exitNode(result);
				return result;
			}

			pushSymbolTableScope();

			for (int i = 0; i < function->paramCount; ++i) {
				variableNode paramDef = function->params[i];
				constantNode* paramVar = &ex(p->functionCall.params[i])->constant;
				constantNode* variableSymbol = addSymbolNode(paramDef);
				variableSymbol->dataType = paramDef.dataType;
				variableSymbol->intVal = paramVar->intVal;
			}

//			debug("\tnode: Function Call %s start\n", name);

			node* res = ex(function->root);

			result->type = typeConstant;
			result->constant.dataType = res->constant.dataType;
			result->constant = res->constant;
			popSymbolTableScope();

//			debug("\tnode: Function Call %s return %s\n", name, constantValueToString(result->constant));
			break;

		case typeEmpty:
			result->type = typeEmpty;
			break;

		default:
			result->type = typeEmpty;
			break;
	}

	exitNode(result);
	return result;
}


