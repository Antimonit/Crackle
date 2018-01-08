#include <stdio.h>
#include <stdlib.h>
#include <headers/types.h>
#include <ast_printer.h>
#include "headers/interpreter.h"
#include "headers/symbol_table.h"
#include "headers/function_symbol_table.h"
#include "headers/node_helpers.h"
#include "y.tab.h"
#include "headers/debug.h"
#include "headers/types.h"


node* var(node* p, node* result) {
	node* varNode = p->oper.op[0];
	variableNode variableNode = varNode->variable;

	constantNode* variable = addSymbolNode(variableNode);
	if (variable == NULL) {
		result->type = typeEmpty;
		return result;
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
			return result;
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

	return result;
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

node* returnx(node* p, node* result) {
	node* value = ex(p->oper.op[0]);

	result->type = typeReturn;
	result->ret.value = value->constant;

//	debug("\tnode: Operand Return %s\n", constantValueToString(result->ret.value));

	return result;
}

node* comma(node* p, node* result) {
	node* left = p->oper.op[0];
	node* right = p->oper.op[1];

	// TODO: reclaim all parameters from the syntax tree

	/*
	if (left->type == typeVariable) {
		node* leftConstant = ex(left);
		node* rightConstant = ex(right);
	} else {
		node* exx = ex(left);
	}
	*/

	debug("\tnode: Operand Comma\n");
	return result;
}

node* whilex(node* p, node* result) {
	while (ex(p->oper.op[0])->constant.intVal) {
		pushSymbolTableScope();
		node* left = ex(p->oper.op[1]);
		if (left->type == typeReturn) {
			result->type = typeReturn;
			result->ret.value = left->ret.value;
			popSymbolTableScope();
//			debug("\tnode: Operand While\n");
			return result;
		}
		popSymbolTableScope();
	}

	result->type = typeOperator;
	result->oper.oper = WHILE;
//	debug("\tnode: Operand While\n");
	return result;
}

node* forx(node* p, node* result) {
	result->type = typeOperator;
	result->oper.oper = FOR;

	for (ex(p->oper.op[0]);
		 ex(p->oper.op[1])->constant.boolVal;
		 ex(p->oper.op[2])) {

		pushSymbolTableScope();
		node* left = ex(p->oper.op[3]);
		if (left->type == typeReturn) {
			popSymbolTableScope();
			result->type = typeReturn;
			result->ret.value = left->ret.value;
			return result;
		}
		popSymbolTableScope();
	}

	return result;
}

node* ifx(node* p, node* result) {
	pushSymbolTableScope();
	if (ex(p->oper.op[0])->constant.intVal) {
		node* left = ex(p->oper.op[1]);
		if (left->type == typeReturn) {
			result->type = left->type;
			result->ret.value = left->ret.value;
			popSymbolTableScope();
//			debug("\tnode: Operand IF\n");
			return result;
		}
	} else if (p->oper.opCount > 2) {
		node* right = ex(p->oper.op[2]);
		if (right->type == typeReturn) {
			result->type = right->type;
			result->ret.value = right->ret.value;
			popSymbolTableScope();
//			debug("\tnode: Operand IF\n");
			return result;
		}
	}

	result->type = typeOperator;
	result->oper.oper = IF;
	popSymbolTableScope();
//	debug("\tnode: Operand IF\n");
	return result;
}

node* delimiter(node* p, node* result) {
	result->type = typeOperator;
	result->oper.oper = ';';

	if (p->oper.opCount > 0) {
		node* left = ex(p->oper.op[0]);
		if (left->type == typeReturn) {
			result->type = typeReturn;
			result->ret.value = left->ret.value;
			return result;
		}
	}
	if (p->oper.opCount > 1) {
		node* right = ex(p->oper.op[1]);
		if (right->type == typeReturn) {
			result->type = typeReturn;
			result->ret.value = right->ret.value;
			return result;
		}
	}
	return result;
}

node* assign(node* p, node* result) {
	node* one = p->oper.op[0];
	variableNode variableNode = one->variable;
	node* value = ex(p->oper.op[1]);

	constantNode* variable = findSymbolNode(variableNode.name);

	if (variable == NULL) {
		printf("Warning: Unable to assign undefined variable '%s'.\n", variableNode.name);
		result->type = typeEmpty;
		return result;
	}

	if (variable->dataType != value->constant.dataType) {
		printf("Warning: Incompatible assignment of type '%s' to variable '%s' of type '%s'.\n",
			   dataTypeToString(value->constant.dataType), variableNode.name, dataTypeToString(variable->dataType));
		result->type = typeEmpty;
		return result;
	}

	const char* name = variableNode.name;
	dataTypeEnum type = variable->dataType;

	result->constant.dataType = type;
	result->constant = *variable = value->constant;

	debug("\tnode: Operand Assign %s %s %s\n",
		  dataTypeToString(value->constant.dataType), name, constantValueToString(value->constant));

	return result;
}

node* uminus(node* p, node* result) {
	node* a = ex(p->oper.op[0]);
	result->type = typeConstant;
	result->constant.dataType = a->constant.dataType;
	if (result->constant.dataType == typeInt) {
		result->constant.intVal = -a->constant.intVal;
		debug("\tnode: Operand Unary Minus %d\n", result->constant.intVal);
	} else if (result->constant.dataType == typeDouble) {
		result->constant.doubleVal = -a->constant.doubleVal;
		debug("\tnode: Operand Unary Minus %f\n", result->constant.doubleVal);
	} else {
		printf("Warning: Invalid argument to unary minus.\n");
	}
	return result;
}

node* plus(node* p, node* result) {
	constantNode* left = &ex(p->oper.op[0])->constant;
	constantNode* right = &ex(p->oper.op[1])->constant;
	result->type = typeConstant;
	result->constant.dataType = widenNodes(left, right);
	if (result->constant.dataType == typeInt) {
		result->constant.intVal = left->intVal + right->intVal;
//		debug("\tnode: Operand Plus %d\n", result->constant.intVal);
	} else if (result->constant.dataType == typeDouble) {
		result->constant.doubleVal = left->doubleVal + right->doubleVal;
//		debug("\tnode: Operand Plus %f\n", result->constant.doubleVal);
	} else {
		printf("Warning: Invalid argument to plus operator.\n");
	}
	return result;
}
node* minus(node* p, node* result) {
	constantNode* left = &ex(p->oper.op[0])->constant;
	constantNode* right = &ex(p->oper.op[1])->constant;
	result->type = typeConstant;
	result->constant.dataType = widenNodes(left, right);
	if (result->constant.dataType == typeInt) {
		result->constant.intVal = left->intVal - right->intVal;
//		debug("\tnode: Operand Minus %d\n", result->constant.intVal);
	} else if (result->constant.dataType == typeDouble) {
		result->constant.doubleVal = left->doubleVal - right->doubleVal;
//		debug("\tnode: Operand Minus %f\n", result->constant.doubleVal);
	} else {
		printf("Warning: Invalid argument to minus operator.\n");
	}
	return result;
}
node* multiply(node* p, node* result) {
	constantNode* left = &ex(p->oper.op[0])->constant;
	constantNode* right = &ex(p->oper.op[1])->constant;
	result->type = typeConstant;
	result->constant.dataType = widenNodes(left, right);
	if (result->constant.dataType == typeInt) {
		result->constant.intVal = left->intVal * right->intVal;
//		debug("\tnode: Operand Multiply %d\n", result->constant.intVal);
	} else if (result->constant.dataType == typeDouble) {
		result->constant.doubleVal = left->doubleVal * right->doubleVal;
//		debug("\tnode: Operand Multiply %f\n", result->constant.doubleVal);
	} else {
		printf("Warning: Invalid argument to multiply operator.\n");
	}
	return result;
}
node* divide(node* p, node* result) {
	constantNode* left = &ex(p->oper.op[0])->constant;
	constantNode* right = &ex(p->oper.op[1])->constant;
	result->type = typeConstant;
	result->constant.dataType = widenNodes(left, right);
	if (result->constant.dataType == typeInt) {
		result->constant.intVal = left->intVal / right->intVal;
//		debug("\tnode: Operand Divide %d\n", result->constant.intVal);
	} else if (result->constant.dataType == typeDouble) {
		result->constant.doubleVal = left->doubleVal / right->doubleVal;
//		debug("\tnode: Operand Divide %f\n", result->constant.doubleVal);
	} else {
		printf("Warning: Invalid argument to divide operator.\n");
	}
	return result;
}
node* modulo(node* p, node* result) {
	constantNode* left = &ex(p->oper.op[0])->constant;
	constantNode* right = &ex(p->oper.op[1])->constant;
	result->type = typeConstant;
	result->constant.dataType = widenNodes(left, right);
	if (result->constant.dataType == typeInt) {
		result->constant.intVal = left->intVal % right->intVal;
//		debug("\tnode: Operand Modulo %d\n", result->constant.intVal);
	} else {
		printf("Warning: Invalid argument to modulo operator.\n");
	}
	return result;
}

node* andx(node* p, node* result) {
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
	return result;
}
node* orx(node* p, node* result) {
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
	return result;
}
node* neg(node* p, node* result) {
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
	return result;
}

node* lt(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = typeBool;
	result->constant.boolVal = (left->constant.intVal < right->constant.intVal) ? true : false;
	return result;
}
node* le(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = typeBool;
	result->constant.boolVal = (left->constant.intVal <= right->constant.intVal) ? true : false;
	return result;
}
node* gt(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = typeBool;
	result->constant.boolVal = (left->constant.intVal > right->constant.intVal) ? true : false;
	return result;
}
node* ge(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = typeBool;
	result->constant.boolVal = (left->constant.intVal >= right->constant.intVal) ? true : false;
	return result;
}
node* eq(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = typeBool;
	result->constant.boolVal = (left->constant.intVal == right->constant.intVal) ? true : false;
	return result;
}
node* ne(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = typeBool;
	result->constant.boolVal = (left->constant.intVal != right->constant.intVal) ? true : false;
	return result;
}

node* print(node* p, node* result) {
	node* value = ex(p->oper.op[0]);

//	debug("\tnode: Print\n");
	switch (value->type) {
		case typeOperator:
			switch (value->oper.oper) {
				case WHILE: printf("WHILE");
							break;
				case IF:	printf("IF");
							break;
				case FUN:	printf("FUN");
							break;
				case VAR:	printf("VAR");
							break;
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
	return result;
}

node* println(node* p, node* result) {
	print(p, result);
	printf("\n");
	return result;
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
			exitNode(result);
			return result;

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
			exitNode(result);
			return result;

		case typeOperator:
			;
			node* ret;
			switch (p->oper.oper) {
				case PRINT:		ret = print(p, result); break;
				case PRINTLN:	ret = println(p, result); break;
				case VAR:		ret = var(p, result); break;
				case RETURN:	ret = returnx(p, result); break;
				case ',':		ret = comma(p, result); break;

				case WHILE:		ret = whilex(p, result); break;
				case IF:		ret = ifx(p, result); break;
				case FOR:		ret = forx(p, result); break;
				case ';':		ret = delimiter(p, result); break;
				case '=':		ret = assign(p, result); break;

				case UMINUS:	ret = uminus(p, result); break;

				case '+':		ret = plus(p, result); break;
				case '-':		ret = minus(p, result); break;
				case '*':		ret = multiply(p, result); break;
				case '/':		ret = divide(p, result); break;
				case '%':		ret = modulo(p, result); break;

				case AND:		ret = andx(p, result); break;
				case OR:		ret = orx(p, result); break;
				case NEG:		ret = neg(p, result); break;

				case LT:		ret = lt(p, result); break;
				case LE:		ret = le(p, result); break;
				case GT:		ret = gt(p, result); break;
				case GE:		ret = ge(p, result); break;
				case EQ:		ret = eq(p, result); break;
				case NE:		ret = ne(p, result); break;
				default:
					printf("Error: unknown operator %d", p->oper.oper);
					ret = result;
			}
			exitNode(ret);
			return ret;

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
				constantNode* variable = addSymbolNode(paramDef);
				variable->dataType = paramDef.dataType;
				variable->intVal = paramVar->intVal;
			}

//			debug("\tnode: Function Call %s start\n", name);

			node* res = ex(function->root);

			result->type = typeConstant;
			result->constant.dataType = res->constant.dataType;
			result->constant = res->constant;
			popSymbolTableScope();

//			debug("\tnode: Function Call %s return %s\n", name, constantValueToString(result->constant));

			exitNode(result);
			return result;

		case typeEmpty:
			result->type = typeEmpty;
			exitNode(result);
			return result;

		default:
//			debug("\tnode: UNDEFINED (%d)\n", p->type);
			result->type = typeEmpty;
			exitNode(result);
			return result;
	}
}


