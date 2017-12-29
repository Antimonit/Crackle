#include <stdio.h>
#include <stdlib.h>
#include <mem.h>
#include "symbols.h"
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
    variableNode variable = p->oper.op[0]->variable;
	node* type = ex(p->oper.op[1]);
	int index = addSymbolNode(variable, type->dataType.type);

    dataTypeEnum dataType = variables[index].type;
    result->constant.type = dataType;

    if (dataType == typeInt) {
        result->constant.intVal = variables[index].intVal = 0;
        debug("\tnode: Operand Var int %s %d\n", variable.name, variables[index].intVal);
    } else if (dataType == typeDouble) {
        result->constant.doubleVal = variables[index].doubleVal = 0.0;
        debug("\tnode: Operand Var double %s %f\n", variable.name, variables[index].doubleVal);
    } else if (dataType == typeString) {
        result->constant.stringVal = variables[index].stringVal = "";
        debug("\tnode: Operand Var string %s %s\n", variable.name, variables[index].stringVal);
    } else if (dataType == typeBool) {
        result->constant.boolVal = variables[index].boolVal = false;
        debug("\tnode: Operand Var bool %s %s\n", variable.name, variables[index].boolVal == true ? "true" : "false");
    }
	return result;
}

node* fun(node *p, node *result) {
	const char* name = p->oper.op[0]->variable.name;
	node* params = ex(p->oper.op[1]);
    node* type = p->oper.op[2];
	node* root = p->oper.op[3];
	
	result->type = typeFunctionDef;
	result->function.name = name;
    result->function.dataType = type->dataType.type;
	result->function.root = root;
//	result->function.params = TODO

	addFuncRoot(name, root);
	
	debug("\tnode: Operand Fun %s\n", name);
	return result;
}

node* returnx(node* p, node* result) {
	node* value = ex(p->oper.op[0]);
	
	result->type = typeReturn;
	result->ret.value = value->constant;
	if (result->ret.value.type == typeInt) {
		debug("\tnode: Operand Return %d\n", result->ret.value.intVal);
	} else if (result->ret.value.type == typeDouble) {
		debug("\tnode: Operand Return %f\n", result->ret.value.doubleVal);
	}
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
	
	int index = findFunctionSymbol(name);
	if (index == -1) {
		printf("Function %s is undefined.\n", name);
		return result;
	}
	
	// TODO: pass arguments to function
		
	node* res = ex(funcRoots[index]);

	result->type = typeConstant;
	result->constant.type = res->constant.type;
	if (result->constant.type == typeInt) {
		result->constant.intVal = res->constant.intVal;
		debug("\tnode: Function Call %s %d\n", name, result->constant.intVal);
	} else if (result->constant.type == typeDouble) {
		result->constant.doubleVal = res->constant.doubleVal;
		debug("\tnode: Function Call %s %f\n", name, result->constant.doubleVal);
	}
	
	return result;
}

node* whilex(node* p, node* result) {
	while (ex(p->oper.op[0])->constant.intVal) {
		node* left = ex(p->oper.op[1]);
		if (left->type == typeReturn) {
			result->type = left->type;
			result->ret.value = left->ret.value;
			debug("\tnode: Operand While\n");
			return result;
		}
	}
	
	result->type = typeOperator;
	result->oper.oper = WHILE;
	debug("\tnode: Operand While\n");
	return result;
}

node* ifx(node* p, node* result) {
	if (ex(p->oper.op[0])->constant.intVal) {
		node* left = ex(p->oper.op[1]);
		if (left->type == typeReturn) {
			result->type = left->type;
			result->ret.value = left->ret.value;
			debug("\tnode: Operand IF\n");
			return result;
		}
	} else if (p->oper.opCount > 2) {
		node* right = ex(p->oper.op[2]);
		if (right->type == typeReturn) {
			result->type = right->type;
			result->ret.value = right->ret.value;
			debug("\tnode: Operand IF\n");
			return result;
		}
	}
	
	result->type = typeOperator;
	result->oper.oper = IF;
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
    variableNode variable = p->oper.op[0]->variable;
	node* value = ex(p->oper.op[1]);
    int index = findTypedSymbolNode(variable, value->constant.type);
    if (index == -1) {
        printf("Symbol not found.\n");
    }

	dataTypeEnum type = variables[index].type;
	result->constant.type = type;
	
	if (type == typeInt) {
		result->constant.intVal = variables[index].intVal = value->constant.intVal;
		debug("\tnode: Operand Assign int %s %d\n", variable.name, value->constant.intVal);
	} else if (type == typeDouble) {
		result->constant.doubleVal = variables[index].doubleVal = value->constant.doubleVal;
		debug("\tnode: Operand Assign double %s %f\n", variable.name, value->constant.doubleVal);
	} else if (type == typeString) {
		result->constant.stringVal = variables[index].stringVal = value->constant.stringVal;
		debug("\tnode: Operand Assign string %s %s\n", variable.name, value->constant.stringVal);
	} else if (type == typeBool) {
		result->constant.boolVal = variables[index].boolVal = value->constant.boolVal;
		debug("\tnode: Operand Assign bool %s %s\n", variable.name, value->constant.boolVal == true ? "true" : "false");
	}
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

	const char* name;
	int index;
	dataTypeEnum type;
	
	node* result = malloc(sizeof(node));
	result->type = typeEmpty;

	if (!p)
		return result;

	switch (p->type) {
		case typeConstant:
			result->type = typeConstant;
            result->constant.type = p->constant.type;
			switch (p->constant.type) {
				case typeInt:		debug("\tnode: Constant Int %d\n", p->constant.intVal);
									result->constant.intVal = p->constant.intVal;
                                    break;
				case typeDouble:	debug("\tnode: Constant Double %f\n", p->constant.doubleVal);
									result->constant.doubleVal = p->constant.doubleVal;
                                    break;
				case typeString:	debug("\tnode: Constant String %s\n", p->constant.stringVal);
									result->constant.stringVal = p->constant.stringVal;
                                    break;
				case typeBool:	    debug("\tnode: Constant Bool %s\n", p->constant.boolVal == true ? "true" : "false");
									result->constant.boolVal = p->constant.boolVal;
                                    break;
			}
            return result;
		
		case typeVariable:
			name = p->variable.name;
			index = findSymbol(name);
			type = variables[index].type;

			result->type = typeConstant;
			result->constant.type = type;

			if (type == typeInt) {
				result->constant.intVal = variables[index].intVal;
				debug("\tnode: Identifier %s %d\n", name, variables[index].intVal);
			} else if (type == typeDouble) {
				result->constant.doubleVal = variables[index].doubleVal;
				debug("\tnode: Identifier %s %f\n", name, variables[index].doubleVal);
			} else if (type == typeString) {
				result->constant.stringVal = variables[index].stringVal;
				debug("\tnode: Identifier %s %s\n", name, variables[index].stringVal);
			} else if (type == typeBool) {
				result->constant.boolVal = variables[index].boolVal;
				debug("\tnode: Identifier %s %s\n", name, variables[index].boolVal == true ? "true" : "false");
			}
			
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