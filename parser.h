//
// Created by DAVE on 31. 12. 2017.
//

#ifndef CRACKLE_INTERPRETER_PARSER_H
#define CRACKLE_INTERPRETER_PARSER_H

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <y.tab.h>
#include <headers/types.hpp>
#include <headers/node_helpers.hpp>
#include "headers/types.hpp"
#include "headers/interpreter.h"
#include "headers/symbol_table.hpp"
#include "headers/debug.h"


int yylex(void);

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


node* newNode() {
	return (node*) malloc(sizeof(node));;
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

node* newEmptyNode() {
	node* empty = newNode();
	empty->type = typeEmpty;
	return empty;
}


int getParamCount(node* params) {
	int paramCount = 0;
	if (params != NULL) {
		paramCount = 1;
		node *countingParams = params;
		while (countingParams->oper.op[0] != NULL) {
			paramCount++;
			countingParams = countingParams->oper.op[0];
		}
	}
	return paramCount;
}

node* function(node* typedVariable, node* params, node* root) {
	/* count number of parameters */
	int paramCount = getParamCount(params);

	/* allocate node, extending params array */
	node* function = malloc(sizeof(node) + paramCount * sizeof(variableDefNode));

	variableNode variable = typedVariable->variable;
	functionDefNode* functionDef = &function->functionDef;

	function->type = typeFunctionDef;
	functionDef->dataType = variable.dataType;
	functionDef->name = variable.name;
	functionDef->root = root;
	functionDef->paramCount = paramCount;

	/* retrieve actual parameters */
	while (paramCount > 0) {
		paramCount--;
		node *p = params->oper.op[1]; // p is variable, not variableDef
//		printf("param: %d | name: %s | type: %s\n", paramCount, p->variable.name, dataTypeToString(p->variable.dataType));
		functionDef->params[paramCount].name = p->variable.name;
		functionDef->params[paramCount].value.dataType = p->variable.dataType;
		params = params->oper.op[0];
	}

	return function;
}

node* functionCall(const char *value, node *params) {
	/* count number of parameters */
	int paramCount = getParamCount(params);

	/* allocate node, extending params array */
	node* functionCall = malloc(sizeof(node) + paramCount * sizeof(constantNode*));

	functionCall->type = typeFunction;
	functionCall->function.name = value;

	/* retrieve actual parameters */
	while (paramCount > 0) {
		paramCount--;
		functionCall->function.params[paramCount] = params->oper.op[1];
		params = params->oper.op[0];
	}

	return functionCall;
}


node* constantInt(int value) {
	node* constant = newNode();
	constant->type = typeConstant;
	constant->constant.dataType = typeInt;
	constant->constant.intVal = value;
	return constant;
}

node* constantDouble(double value) {
	node* constant = newNode();
	constant->type = typeConstant;
	constant->constant.dataType = typeDouble;
	constant->constant.doubleVal = value;
	return constant;
}

node* constantString(const char* value) {
	node* constant = newNode();
	constant->type = typeConstant;
	constant->constant.dataType = typeString;
	constant->constant.stringVal = value;
	return constant;
}

node* constantBool(bool value) {
	node* constant = newNode();
	constant->type = typeConstant;
	constant->constant.dataType = typeBool;
	constant->constant.boolVal = value;
	return constant;
}

node* constantNull() {
	node* constant = newNode();
	constant->type = typeConstant;
	constant->constant.dataType = typeObj;
	constant->constant.objectVal = NULL;
	constant->constant.objectTypeName = "null";
	return constant;
}


node* objectDef(const char* name, node* vars) {
	/* count number of vars */
	int varCount = getParamCount(vars);

	/* allocate node, extending params array */
	node* result = malloc(sizeof(node) + varCount * sizeof(variableDefNode));

	result->type = typeObjectDef;

	result->objectDef.name = name;
	result->objectDef.varCount = varCount;
	while (varCount > 0) {
		varCount--;
		node *p = vars->oper.op[1]; // p is variableDef
		result->objectDef.vars[varCount] = p->variableDef;
		vars = vars->oper.op[0];
	}

	return result;
}

node* object(const char* name) {
	node* result = newNode();
	result->type = typeObject;
	result->object.name = name;
	return result;
}


node* variableDef(node* typedVariable, node* defaultValue) {
	node* result = newNode();
	result->type = typeVariableDef;

	variableDefNode* variableDef = &result->variableDef;

	variableDef->name = typedVariable->variable.name;
	variableDef->value.dataType = typedVariable->variable.dataType;
	variableDef->defaultValue = defaultValue;

	return result;
}

node* typedVariable(const char* name, dataTypeEnum type) {
	node* variable = newNode();
	variable->type = typeVariable;
	variable->variable.dataType = type;
	variable->variable.name = name;
	return variable;
}

node* variable(const char* name) {
	return typedVariable(name, typeUndefined);
}

node* op(int oper, int opCount, ...) {
	/* allocate node, extending op array */
	node* operand = malloc(sizeof(node) + opCount * sizeof(node*));

	operand->type = typeOperator;
	operand->oper.oper = oper;
	operand->oper.opCount = opCount;

	va_list ap;
	va_start(ap, opCount);
	for (int i = 0; i < opCount; i++) {
		operand->oper.op[i] = va_arg(ap, node*);
	}
	va_end(ap);

	return operand;
}

#endif //CRACKLE_INTERPRETER_PARSER_H
