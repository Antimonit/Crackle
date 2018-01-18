#include <stdio.h>
#include <stdlib.h>
#include <headers/types.hpp>
#include <headers/ast_printer.hpp>
#include "headers/interpreter.hpp"
#include "headers/symbol_table.hpp"
#include "headers/node_helpers.hpp"
#include "parser.tab.hh"

using token = MC::Parser::token;

void returnx(node* p, node* result) {
	node* value = ex(p->oper.op[0]);

	result->type = typeReturn;
	result->ret.value = value->constant;
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
	result->oper.oper = token::WHILE;
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
	result->oper.oper = token::FOR;
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
	result->oper.oper = token::IF;
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

	constantNode* variable = findVariable(variableNode.name);

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

	result->type = typeConstant;
	result->constant.dataType = type;
	result->constant = *variable = value->constant;
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
				case token::FOR:	printf("FOR");		break;
				case token::WHILE:	printf("WHILE");	break;
				case token::IF:	printf("IF");		break;
				case token::FUN:	printf("FUN");		break;
				case token::VAR:	printf("VAR");		break;
				default:	printf("WRONG OPERATOR");
			}
			break;
		case typeConstant:
			printf("%s", constantValueToString(value->constant));
			break;
		case typeFunctionDef:
			printf("Function %s defined", value->functionDef.name);
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
	node* result = (node*) malloc(sizeof(node));
	result->type = typeEmpty;

	if (!p)
		return result;

	enterNode(p);

	switch (p->type) {
		case typeConstant: {
			result->type = typeConstant;
			result->constant = p->constant;
			break;
		}
		case typeVariableDef: {
			variableDefNode *variableDef = &p->variableDef;

			if (variableDef->defaultValue != NULL) {
				node* defaultValue = ex(variableDef->defaultValue);
				if (variableDef->value.dataType != defaultValue->constant.dataType) {
					printf("Warning: Defined value of type %s is incompatible with variable type %s\n",
						   dataTypeToString(variableDef->value.dataType),
						   dataTypeToString(defaultValue->constant.dataType));
				} else {
					variableDef->value = defaultValue->constant;
				}
			} else {
				defaultConstant(&variableDef->value);
			}

			addVariable(variableDef);
			break;
		}
		case typeVariable: {
			result->type = typeConstant;

			const char* name = p->variable.name;
			constantNode* variable = findVariable(name);

			if (variable == NULL) {
				printf("Warning: Undefined variable '%s'.\n", name);
				result->type = typeEmpty;
				exitNode(result);
				return result;
			}

			result->constant = *variable;
			break;
		}
		case typeOperator: {
			switch (p->oper.oper) {
				case token::PRINT:		print(p, result); break;
				case token::PRINTLN:	println(p, result); break;
				case token::RETURN:	returnx(p, result); break;

				case token::WHILE:		whilex(p, result); break;
				case token::IF:		ifx(p, result); break;
				case token::FOR:		forx(p, result); break;
				case ';':		delimiter(p, result); break;
				case '=':		assign(p, result); break;

				case token::UMINUS:	uminus(p, result); break;

				case '+':		plus(p, result); break;
				case '-':		minus(p, result); break;
				case '*':		multiply(p, result); break;
				case '/':		divide(p, result); break;
				case '%':		modulo(p, result); break;

				case token::AND:		andx(p, result); break;
				case token::OR:		orx(p, result); break;
				case token::NEG:		neg(p, result); break;

				case token::LT:		lt(p, result); break;
				case token::LE:		le(p, result); break;
				case token::GT:		gt(p, result); break;
				case token::GE:		ge(p, result); break;
				case token::EQ:		eq(p, result); break;
				case token::NE:		ne(p, result); break;
				default:		printf("Error: unknown operator %d", p->oper.oper);
			}
			break;
		}
		case typeFunctionDef: {
			addFunction(&p->functionDef);
			break;
		}
		case typeFunction: {
			const char* name = p->function.name;

			functionDefNode* function = findFunction(name);
			if (function == NULL) {
				printf("Warning: Undefined function '%s'.\n", name);
				exitNode(result);
				return result;
			}

			for (int i = 0; i < function->paramCount; ++i) {
				variableDefNode* paramDef = &function->params[i];
				constantNode* paramVar = &ex(p->function.params[i])->constant;
				if (paramDef->value.dataType == paramVar->dataType) {
					paramDef->value = *paramVar;
				} else {
					defaultConstant(&paramDef->value);
					printf("Warning: Passing incompatible parameter of type %s instead of type %s\n",
						   dataTypeToString(paramVar->dataType), dataTypeToString(paramDef->value.dataType));
				}
			}

			replaceSymbolTableScope();
			for (int i = 0; i < function->paramCount; ++i) {
				addVariable(&function->params[i]);
			}
			node* res = ex(function->root);
			popSymbolTableScope();

			result->type = typeConstant;
			result->constant = res->constant;
			break;
		}
		case typeObjectDef: {
			addObject(&p->objectDef);
			break;
		}
		case typeObject: {
			const char* name = p->object.name;

			objectDefNode *object = findObject(name);
			if (object == NULL) {
				printf("Warning: Undefined object '%s'.\n", name);
				result->type = typeEmpty;
				exitNode(result);
				return result;
			}

			result->type = typeConstant;
			result->constant.dataType = typeObj;
			result->constant.objectTypeName = name;
			result->constant.objectVal = (objectDefNodeTag*) malloc(object->varCount * sizeof(constantNode*));

			objectDefNode* objectVal = result->constant.objectVal;
//			constantNode** objectVal = result->constant.objectVal;
			for (int i = 0; i < object->varCount; ++i) {
				variableDefNode* paramDef = &object->vars[i];
				constantNode *ptr = &p->object.vars[i];
//				objectVal[i] = ptr;
//				if (paramDef->value.dataType == paramVar->dataType) {
//					paramDef->value = *paramVar;
//				} else {
//					defaultConstant(&paramDef->value);
//					printf("Warning: Passing incompatible parameter of type %s instead of type %s\n",
//						   dataTypeToString(paramVar->dataType), dataTypeToString(paramDef->value.dataType));
//				}
			}
			result->constant.objectVal = objectVal;

			break;
		}
		case typeEmpty:
			break;
		default:
			break;
	}

	exitNode(result);
	return result;
}


