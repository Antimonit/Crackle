#include <stdio.h>
#include <stdlib.h>
#include <mem.h>
#include "symbol_table.h"
#include "function_symbol_table.h"
#include "types.h"
#include "y.tab.h"
#include "debug.h"


node* ex(node* p);


dataTypeEnum maxType(node* a, node* b) {
	int typeA = a->constant.type;
	int typeB = b->constant.type;
	if (typeA == typeInt && typeB == typeInt) {
		return typeInt;
	} else {
		return typeDouble;
	}
}

void widenNode(node* a, int maxType) {
	if (a->constant.type == maxType)
		return;
	if (a->constant.type == typeInt && maxType == typeDouble) {
		a->constant.type = typeDouble;
		a->constant.doubleVal = (double) a->constant.intVal;
	} else {
		printf("ERROR: narrowing instead of widening???\n");
	}
}

dataTypeEnum widenNodes(node* a, node* b) {
    dataTypeEnum max = maxType(a, b);
	widenNode(a, max);
	widenNode(b, max);
	return max;
}


node* var(node* p, node* result) {
    variableNode variableNode = p->oper.op[0]->variable;
	node* typeNode = ex(p->oper.op[1]);
	constantNode* variable = addSymbolNode(variableNode, typeNode->dataType.type);
    if (variable == NULL) {
        // TODO
        return result;
    }
	const char* name = variableNode.name;
    dataTypeEnum type = variable->type;

    result->constant.type = type;
    if (type == typeInt) {
        result->constant.intVal = variable->intVal = 0;
        debug("\tnode: Operand Var int %s %d\n", name, variable->intVal);
    } else if (type == typeDouble) {
        result->constant.doubleVal = variable->doubleVal = 0.0;
        debug("\tnode: Operand Var double %s %f\n", name, variable->doubleVal);
    } else if (type == typeString) {
        result->constant.stringVal = variable->stringVal = "";
        debug("\tnode: Operand Var string %s %s\n", name, variable->stringVal);
    } else if (type == typeBool) {
        result->constant.boolVal = variable->boolVal = false;
        debug("\tnode: Operand Var bool %s %s\n", name, variable->boolVal == true ? "true" : "false");
    }
	return result;
}

node* fun(node *p, node *result) {
	variableNode variableNode = p->oper.op[0]->variable;
	const char* name = variableNode.name;
	node* params = ex(p->oper.op[1]);
    node* type = p->oper.op[2];
	node* root = p->oper.op[3];
	
	result->type = typeFunctionDef;
	result->function.name = name;
    result->function.dataType = type->dataType.type;
	result->function.root = root;
//	result->function.params = TODO

	addFunctionRoot(name, root);
	
	debug("\tnode: Operand Fun %s\n", name);
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

node* functionCall(node* p, node* result) {
	const char* name = p->function.name;
	
	node* root = findFunctionRoot(name);
	if (root == NULL) {
		printf("Function %s is undefined.\n", name);
		return result;
	}

	pushSymbolTableScope();

	// TODO: pass arguments to function
    node* res = ex(root);

	result->type = typeConstant;
	result->constant.type = res->constant.type;
    result->constant = res->constant;
	popSymbolTableScope();
    debug("\tnode: Function Call %s %s\n", name, getConstantValueString(result->constant));

	return result;
}

node* whilex(node* p, node* result) {
	pushSymbolTableScope();
	while (ex(p->oper.op[0])->constant.intVal) {
		node* left = ex(p->oper.op[1]);
		if (left->type == typeReturn) {
			result->type = left->type;
			result->ret.value = left->ret.value;
			popSymbolTableScope();
			debug("\tnode: Operand While\n");
			return result;
		}
	}
	popSymbolTableScope();

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
    if (p->oper.opCount > 0) {
        node *left = ex(p->oper.op[0]);
        if (left->type == typeReturn) {
            result->type = typeReturn;
            result->ret.value = left->ret.value;
            return result;
        }
    }
    if (p->oper.opCount > 1) {
        node *right = ex(p->oper.op[1]);
        if (right->type == typeReturn) {
            result->type = typeReturn;
            result->ret.value = right->ret.value;
            return result;
        }
    }
	result->type = typeOperator;
	result->oper.oper = ';';
	debug("\tnode: Operand Delimiter\n");
	return result;
}

node* assign(node* p, node* result) {
	variableNode variableNode = p->oper.op[0]->variable;
	node* value = ex(p->oper.op[1]);
	constantNode* variable = findTypedSymbolNode(variableNode, value->dataType.type);
    if (variable == NULL) {
        printf("Undefined variable '%s'.\n", variableNode.name);
		return result;
    }

	const char* name = variableNode.name;
	dataTypeEnum type = variable->type;

	result->constant.type = type;
    result->constant = *variable = value->constant;

    debug("\tnode: Operand Assign %s %s %s\n",
          getDataTypeString(value->constant.type), name, getConstantValueString(value->constant));

	return result;
}

node* uminus(node* p, node* result) {
	node* a = ex(p->oper.op[0]);
	result->constant.type = a->constant.type;
	if (result->constant.type == typeInt) {
		result->constant.intVal = -a->constant.intVal;
		debug("\tnode: Operand Unary Minus %d\n", result->constant.intVal);
	} else if (result->constant.type == typeDouble) {
		result->constant.doubleVal = -a->constant.doubleVal;
		debug("\tnode: Operand Unary Minus %f\n", result->constant.doubleVal);
	}
	return result;
}

node* plus(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->constant.type = widenNodes(left, right);
	if (result->constant.type == typeInt) {
		result->constant.intVal = left->constant.intVal + right->constant.intVal;
		debug("\tnode: Operand Plus %d\n", result->constant.intVal);
	} else if (result->constant.type == typeDouble) {
		result->constant.doubleVal = left->constant.doubleVal + right->constant.doubleVal;
		debug("\tnode: Operand Plus %f\n", result->constant.doubleVal);
	}
	return result;
}
node* minus(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->constant.type = widenNodes(left, right);
	if (result->constant.type == typeInt) {
		result->constant.intVal = left->constant.intVal - right->constant.intVal;
		debug("\tnode: Operand Minus %d\n", result->constant.intVal);
	} else if (result->constant.type == typeDouble) {
		result->constant.doubleVal = left->constant.doubleVal - right->constant.doubleVal;
		debug("\tnode: Operand Minus %f\n", result->constant.doubleVal);
	}
	return result;
}
node* multiply(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->constant.type = widenNodes(left, right);
	if (result->constant.type == typeInt) {
		result->constant.intVal = left->constant.intVal * right->constant.intVal;
		debug("\tnode: Operand Multiply %d\n", result->constant.intVal);
	} else if (result->constant.type == typeDouble) {
		result->constant.doubleVal = left->constant.doubleVal * right->constant.doubleVal;
		debug("\tnode: Operand Multiply %f\n", result->constant.doubleVal);
    }

	return result;
}
node* divide(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->constant.type = widenNodes(left, right);
	if (result->constant.type == typeInt) {
		result->constant.intVal = left->constant.intVal / right->constant.intVal;
		debug("\tnode: Operand Divide %d\n", result->constant.intVal);
	} else if (result->constant.type == typeDouble) {
		result->constant.doubleVal = left->constant.doubleVal / right->constant.doubleVal;
		debug("\tnode: Operand Divide %f\n", result->constant.doubleVal);
	}
	return result;
}

node* andx(node* p, node* result) {
    node* left = ex(p->oper.op[0]);
    node* right = ex(p->oper.op[1]);
    result->constant.boolVal = ((left->constant.boolVal == true) && (right->constant.boolVal == true)) ? true : false;
    debug("\tnode: Operand OR\n");
    return result;
}
node* orx(node* p, node* result) {
    node* left = ex(p->oper.op[0]);
    node* right = ex(p->oper.op[1]);
    result->constant.boolVal = ((left->constant.boolVal == true) || (right->constant.boolVal == true)) ? true : false;
    debug("\tnode: Operand OR\n");
    return result;
}
node* neg(node* p, node* result) {
    node* value = ex(p->oper.op[0]);
    result->constant.boolVal = (value->constant.boolVal) == true ? false : true;
    debug("\tnode: Operand NEG\n");
    return result;
}

node* lt(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->constant.boolVal = (left->constant.intVal < right->constant.intVal) ? true : false;
	debug("\tnode: Operand LT\n");
	return result;
}
node* le(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->constant.boolVal = (left->constant.intVal <= right->constant.intVal) ? true : false;
	debug("\tnode: Operand LE\n");
	return result;
}
node* gt(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->constant.boolVal = (left->constant.intVal > right->constant.intVal) ? true : false;
	debug("\tnode: Operand GT\n");
	return result;
}
node* ge(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->constant.boolVal = (left->constant.intVal >= right->constant.intVal) ? true : false;
	debug("\tnode: Operand GE\n");
	return result;
}
node* eq(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->constant.boolVal = (left->constant.intVal == right->constant.intVal) ? true : false;
	debug("\tnode: Operand EQ\n");
	return result;
}
node* ne(node* p, node* result) {
	node* left = ex(p->oper.op[0]);
	node* right = ex(p->oper.op[1]);
	result->constant.boolVal = (left->constant.intVal != right->constant.intVal) ? true : false;
	debug("\tnode: Operand NE\n");
	return result;
}

node* int_type(node* p, node* result) {
    result->dataType.type = typeInt;
	debug("\tnode: Operand INT_TYPE\n");
	return result;
}
node* double_type(node* p, node* result) {
    result->dataType.type = typeDouble;
	debug("\tnode: Operand DOUBLE_TYPE\n");
	return result;
}
node* bool_type(node* p, node* result) {
    result->dataType.type = typeBool;
	debug("\tnode: Operand BOOL_TYPE\n");
	return result;
}
node* string_type(node* p, node* result) {
    result->dataType.type = typeString;
	debug("\tnode: Operand STRING_TYPE\n");
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
            debug("\tnode: Constant %s %s\n", getDataTypeString(p->constant.type), getConstantValueString(p->constant));
            return result;
		
		case typeVariable:
            result->type = typeConstant;

			const char* name = p->variable.name;
			constantNode* variable = findSymbolNode(name);

            result->constant = *variable;

            debug("\tnode: Identifier %s %s\n", name, getConstantValueString(*variable));
			return result;

		case typeOperator:
			result->type = typeConstant;
			result->constant.type = typeInt;
			result->constant.intVal = 0;
			
			switch (p->oper.oper) {
				case VAR:		return var(p, result);
				case FUN:		return fun(p, result);
				case RETURN:	return returnx(p, result);
				case ',':		return comma(p, result);

				case WHILE:		return whilex(p, result);
				case IF:		return ifx(p, result);
		        case ';':		return delimiter(p, result);
				case ASSIGN:	return assign(p, result);

				case UMINUS:	return uminus(p, result);

				case '+':		return plus(p, result);
				case '-':		return minus(p, result);
				case '*':		return multiply(p, result);
				case '/':		return divide(p, result);

                case AND:       return andx(p, result);
                case OR:        return orx(p, result);
                case NEG:       return neg(p, result);

				case LT:		return lt(p, result);
				case LE:		return le(p, result);
				case GT:		return gt(p, result);
				case GE:		return ge(p, result);
				case EQ:		return eq(p, result);
				case NE:		return ne(p, result);

                case INT_TYPE:      return int_type(p, result);
                case DOUBLE_TYPE:   return double_type(p, result);
                case BOOL_TYPE:     return bool_type(p, result);
                case STRING_TYPE:   return string_type(p, result);
			}
			
		case typeFunctionCall:
			return functionCall(p, result);
		
		case typeEmpty:
			result->type = typeEmpty;
			return result;
		
		default:
//			debug("\tnode: UNDEFINED (%d)\n", p->type);
			result->type = typeEmpty;
			return result;
	}
}