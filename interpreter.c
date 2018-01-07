#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"
#include "symbol_table.h"
#include "function_symbol_table.h"
#include "node_helpers.h"
#include "y.tab.h"
#include "debug.h"


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

	if (p->oper.opCount > 1) {
		node* defaultValue = ex(p->oper.op[1]);
		dataTypeEnum exType = defaultValue->constant.dataType;
		if (varType != exType) {
			printf("Warning: Incompatible assignment of type '%s' to type '%s'\n",
				getDataTypeString(exType), getDataTypeString(varType));
			result->type = typeEmpty;
			return result;
		}

		result->constant = *variable = defaultValue->constant;
		debug("\tnode: Operand Var %s %s %s\n",
			  getDataTypeString(result->constant.dataType), name, getConstantValueString(*variable));

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

node* returnx(node* p, node* result) {
	node* value = ex(p->oper.op[0]);

	result->type = typeReturn;
	result->ret.value = value->constant;

	debug("\tnode: Operand Return %s\n", getConstantValueString(result->ret.value));

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
			result->type = left->type;
			result->ret.value = left->ret.value;
			popSymbolTableScope();
			debug("\tnode: Operand While\n");
			return result;
		}
		popSymbolTableScope();
	}

	result->type = typeOperator;
	result->oper.oper = WHILE;
	debug("\tnode: Operand While\n");
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
			debug("\tnode: Operand IF\n");
			return result;
		}
	} else if (p->oper.opCount > 2) {
		node* right = ex(p->oper.op[2]);
		if (right->type == typeReturn) {
			result->type = right->type;
			result->ret.value = right->ret.value;
			popSymbolTableScope();
			debug("\tnode: Operand IF\n");
			return result;
		}
	}

	result->type = typeOperator;
	result->oper.oper = IF;
	popSymbolTableScope();
	debug("\tnode: Operand IF\n");
	return result;
}

node* delimiter(node* p, node* result) {
//	debug("\tnode: Operand Delimiter\n");
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
	result->type = typeOperator;
	result->oper.oper = ';';
	return result;
}

node* assign(node* p, node* result) {
	variableNode variableNode = p->oper.op[0]->variable;
	node* value = ex(p->oper.op[1]);

	constantNode* variable = findSymbolNode(variableNode.name);

	if (variable == NULL) {
		printf("Warning: Unable to assign undefined variable '%s'.\n", variableNode.name);
		result->type = typeEmpty;
		return result;
	}

	if (variable->dataType != value->constant.dataType) {
		printf("Warning: Incompatible assignment of type '%s' to variable '%s' of type '%s'.\n",
			   getDataTypeString(value->constant.dataType), variableNode.name, getDataTypeString(variable->dataType));
		result->type = typeEmpty;
		return result;
	}

	const char* name = variableNode.name;
	dataTypeEnum type = variable->dataType;

	result->constant.dataType = type;
	result->constant = *variable = value->constant;

	debug("\tnode: Operand Assign %s %s %s\n",
		  getDataTypeString(value->constant.dataType), name, getConstantValueString(value->constant));

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
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = widenNodes(left, right);
	if (result->constant.dataType == typeInt) {
		result->constant.intVal = left->constant.intVal + right->constant.intVal;
		debug("\tnode: Operand Plus %d\n", result->constant.intVal);
	} else if (result->constant.dataType == typeDouble) {
		result->constant.doubleVal = left->constant.doubleVal + right->constant.doubleVal;
		debug("\tnode: Operand Plus %f\n", result->constant.doubleVal);
	} else {
		printf("Warning: Invalid argument to plus operator.\n");
	}
	return result;
}
node* minus(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = widenNodes(left, right);
	if (result->constant.dataType == typeInt) {
		result->constant.intVal = left->constant.intVal - right->constant.intVal;
		debug("\tnode: Operand Minus %d\n", result->constant.intVal);
	} else if (result->constant.dataType == typeDouble) {
		result->constant.doubleVal = left->constant.doubleVal - right->constant.doubleVal;
		debug("\tnode: Operand Minus %f\n", result->constant.doubleVal);
	} else {
		printf("Warning: Invalid argument to minus operator.\n");
	}
	return result;
}
node* multiply(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = widenNodes(left, right);
	if (result->constant.dataType == typeInt) {
		result->constant.intVal = left->constant.intVal * right->constant.intVal;
		debug("\tnode: Operand Multiply %d\n", result->constant.intVal);
	} else if (result->constant.dataType == typeDouble) {
		result->constant.doubleVal = left->constant.doubleVal * right->constant.doubleVal;
		debug("\tnode: Operand Multiply %f\n", result->constant.doubleVal);
	} else {
		printf("Warning: Invalid argument to multiply operator.\n");
	}
	return result;
}
node* divide(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = widenNodes(left, right);
	if (result->constant.dataType == typeInt) {
		result->constant.intVal = left->constant.intVal / right->constant.intVal;
		debug("\tnode: Operand Divide %d\n", result->constant.intVal);
	} else if (result->constant.dataType == typeDouble) {
		result->constant.doubleVal = left->constant.doubleVal / right->constant.doubleVal;
		debug("\tnode: Operand Divide %f\n", result->constant.doubleVal);
	} else {
		printf("Warning: Invalid argument to divide operator.\n");
	}
	return result;
}
node* modulo(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = widenNodes(left, right);
	if (result->constant.dataType == typeInt) {
		result->constant.intVal = left->constant.intVal % right->constant.intVal;
		debug("\tnode: Operand Modulo %d\n", result->constant.intVal);
	} else {
		printf("Warning: Invalid argument to modulo operator.\n");
	}
	return result;
}

node* andx(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = typeBool;
	result->constant.boolVal = ((left->constant.boolVal == true) && (right->constant.boolVal == true)) ? true : false;
	debug("\tnode: Operand OR\n");
	return result;
}
node* orx(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = typeBool;
	result->constant.boolVal = ((left->constant.boolVal == true) || (right->constant.boolVal == true)) ? true : false;
	debug("\tnode: Operand OR\n");
	return result;
}
node* neg(node* p, node* result) {
	node* value = ex(p->oper.op[0]);
	result->type = typeConstant;
	result->constant.dataType = typeBool;
	result->constant.boolVal = (value->constant.boolVal) == true ? false : true;
	debug("\tnode: Operand NEG\n");
	return result;
}

node* lt(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = typeBool;
	result->constant.boolVal = (left->constant.intVal < right->constant.intVal) ? true : false;
	debug("\tnode: Operand LT\n");
	return result;
}
node* le(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = typeBool;
	result->constant.boolVal = (left->constant.intVal <= right->constant.intVal) ? true : false;
	debug("\tnode: Operand LE\n");
	return result;
}
node* gt(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = typeBool;
	result->constant.boolVal = (left->constant.intVal > right->constant.intVal) ? true : false;
	debug("\tnode: Operand GT\n");
	return result;
}
node* ge(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = typeBool;
	result->constant.boolVal = (left->constant.intVal >= right->constant.intVal) ? true : false;
	debug("\tnode: Operand GE\n");
	return result;
}
node* eq(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = typeBool;
	result->constant.boolVal = (left->constant.intVal == right->constant.intVal) ? true : false;
	debug("\tnode: Operand EQ\n");
	return result;
}
node* ne(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->type = typeConstant;
	result->constant.dataType = typeBool;
	result->constant.boolVal = (left->constant.intVal != right->constant.intVal) ? true : false;
	debug("\tnode: Operand NE\n");
	return result;
}

node* print(node* p, node* result) {
	node* value = ex(p->oper.op[0]);

	debug("\tnode: Print\n");

	switch (value->type) {
		case typeOperator:
			switch (value->oper.oper) {
				case WHILE: printf("WHILE\n");
							break;
				case IF:	printf("IF\n");
							break;
				case FUN:	printf("FUN\n");
							break;
				case VAR:	printf("VAR\n");
							break;
				default:	printf("WRONG OPERATOR\n");
			}
			break;
		case typeConstant:
			printf("%s", getConstantValueString(value->constant));
			break;
		case typeFunctionDef:
			printf("Function %s defined\n", value->function.name);
			break;
		case typeEmpty:
			break;
		default:
			printf("WRONG TYPE\n");
	}

	result->type = typeEmpty;
	return result;
}

node* ex(node* p) {
	node* result = malloc(sizeof(node));
	result->type = typeEmpty;

	if (!p)
		return result;

	switch (p->type) {
		case typeConstant:
			result->type = typeConstant;
			result->constant = p->constant;
			debug("\tnode: Constant %s %s\n", getDataTypeString(p->constant.dataType), getConstantValueString(p->constant));
			return result;

		case typeVariable:
			result->type = typeConstant;

			const char* name = p->variable.name;
			constantNode* variable = findSymbolNode(name);

			if (variable == NULL) {
				printf("Warning: Undefined variable '%s'.\n", name);
				result->type = typeEmpty;
				return result;
			}

			result->constant = *variable;

			debug("\tnode: Identifier %s %s\n", name, getConstantValueString(*variable));
			return result;

		case typeOperator:
			switch (p->oper.oper) {
				case PRINT:		return print(p, result);
				case VAR:		return var(p, result);
				case RETURN:	return returnx(p, result);
				case ',':		return comma(p, result);

				case WHILE:		return whilex(p, result);
				case IF:		return ifx(p, result);
				case ';':		return delimiter(p, result);
				case '=':		return assign(p, result);

				case UMINUS:	return uminus(p, result);

				case '+':		return plus(p, result);
				case '-':		return minus(p, result);
				case '*':		return multiply(p, result);
				case '/':		return divide(p, result);
				case '%':		return modulo(p, result);

				case AND:       return andx(p, result);
				case OR:        return orx(p, result);
				case NEG:       return neg(p, result);

				case LT:		return lt(p, result);
				case LE:		return le(p, result);
				case GT:		return gt(p, result);
				case GE:		return ge(p, result);
				case EQ:		return eq(p, result);
				case NE:		return ne(p, result);
			}

		case typeFunctionCall:
			name = p->functionCall.name;

			functionNode* function = findFunction(name);
			if (function == NULL) {
				printf("Warning: Undefined function '%s'.\n", name);
				return result;
			}

			pushSymbolTableScope();

			for (int i = 0; i < function->paramCount; ++i) {
				variableNode paramDef = function->params[i];
				constantNode* paramVar = &ex(p->functionCall.params[i])->constant;
				constantNode* variable = addSymbolNode(paramDef);
				variable->dataType = paramVar->dataType;
				variable->intVal = paramVar->intVal;
			}

			debug("\tnode: Function Call %s start\n", name);

			node* res = ex(function->root);

			result->type = typeConstant;
			result->constant.dataType = res->constant.dataType;
			result->constant = res->constant;
			popSymbolTableScope();

			debug("\tnode: Function Call %s return %s\n", name, getConstantValueString(result->constant));

			return result;

		case typeEmpty:
			result->type = typeEmpty;
			return result;

		default:
//			debug("\tnode: UNDEFINED (%d)\n", p->type);
			result->type = typeEmpty;
			return result;
	}
}


