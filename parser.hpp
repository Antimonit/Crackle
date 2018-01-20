//
// Created by DAVE on 31. 12. 2017.
//

#ifndef CRACKLE_INTERPRETER_PARSER_H
#define CRACKLE_INTERPRETER_PARSER_H

#include <cstdio>
#include <cstdarg>
#include <cmath>
#include <string>
#include <parser.tab.hh>
#include <headers/types.hpp>
#include <headers/node_helpers.hpp>
#include <utility>
#include <c++/iostream>
#include "headers/types.hpp"
#include "headers/symbol_table.hpp"
#include "headers/debug.h"


int yyerror(const std::string &message) {
	std::cerr << message << std::endl;
	return 1;
}


Node* newNode() {
	return (Node*) malloc(10*sizeof(Node));;
}

void freeNode(Node* node) {
	if (!node)
		return;

	if (node->type == typeOperator) {
		for (int i = 0; i < node->oper.opCount; i++) {
			freeNode(node->oper.op[i]);
		}
	}
	free(node);
}


int getParamCount(Node* params) {
	int paramCount = 0;
	if (params != nullptr) {
		paramCount = 1;
		Node *countingParams = params;
		while (countingParams->oper.op[0] != nullptr) {
			paramCount++;
			countingParams = countingParams->oper.op[0];
		}
	}
	return paramCount;
}

Node* function(Node* typedVariable, Node* params, Node* root) {
	/* count number of parameters */
	int paramCount = getParamCount(params);

	/* allocate node, extending params array */
//	Node* function = (Node*) malloc(sizeof(Node) + paramCount * sizeof(VariableDefNode));
	Node* function = newNode();

//	ObjectNode* objectNode = (ObjectNode*) operator new (offsetof (ObjectNode, vars[3]));

	VariableNode variable = typedVariable->variable;
	FunctionDefNode* functionDef = &function->functionDef;

	function->type = typeFunctionDef;
	functionDef->dataType = variable.dataType;
	functionDef->name = variable.name;
	functionDef->root = root;
	functionDef->paramCount = paramCount;

	/* retrieve actual parameters */
	while (paramCount > 0) {
		paramCount--;
		Node *p = params->oper.op[1]; // p is variable, not variableDef
//		printf("param: %d | name: %s | type: %s\n", paramCount, p->variable.name, dataTypeToString(p->variable.dataType));
		functionDef->params[paramCount].name = p->variable.name;
		functionDef->params[paramCount].value.dataType = p->variable.dataType;
		params = params->oper.op[0];
	}

	return function;
}

Node* functionCall(const std::string &name, Node *params) {
	/* count number of parameters */
	int paramCount = getParamCount(params);

	/* allocate node, extending params array */
	Node* functionCall = (Node*) malloc(sizeof(Node) + paramCount * sizeof(ConstantNode*));
//	Node* functionCall = new (typeFunction, paramCount) Node;

	functionCall->type = typeFunction;
	functionCall->function.name = name;

	/* retrieve actual parameters */
	while (paramCount > 0) {
		paramCount--;
		functionCall->function.params[paramCount] = params->oper.op[1];
		params = params->oper.op[0];
	}

	return functionCall;
}


Node* constantInt(int value) {
//	Node* constant = new (typeConstant) Node;
	Node* constant = newNode();
	constant->type = typeConstant;
	constant->constant.dataType = typeInt;
	constant->constant.intVal = value;
	return constant;
}

Node* constantDouble(double value) {
	Node* constant = newNode();
	constant->type = typeConstant;
	constant->constant.dataType = typeDouble;
	constant->constant.doubleVal = value;
	return constant;
}

Node* constantString(const std::string &value) {
	Node* constant = newNode();
	constant->type = typeConstant;
//	new ((void*) &constant->constant) ConstantNode;
	constant->constant.dataType = typeString;
//	new ((void*) &constant->constant.stringVal) std::string(value);
	constant->constant.stringVal = value;
	return constant;
}

Node* constantBool(bool value) {
	Node* constant = newNode();
	constant->type = typeConstant;
	constant->constant.dataType = typeBool;
	constant->constant.boolVal = value;
	return constant;
}

Node* constantNull() {
	Node* constant = newNode();
	constant->type = typeConstant;
	constant->constant.dataType = typeObj;
	constant->constant.objectVal = nullptr;
	constant->constant.objectTypeName = "null";
	return constant;
}


Node* objectDef(const std::string &name, Node* vars) {
	/* count number of vars */
	int varCount = getParamCount(vars);

	/* allocate node, extending params array */
	Node* result = (Node*) malloc(sizeof(Node) + varCount * sizeof(VariableDefNode));

	result->type = typeObjectDef;

	result->objectDef.name = name;
	result->objectDef.varCount = varCount;
	while (varCount > 0) {
		varCount--;
		Node *p = vars->oper.op[1]; // p is variableDef
		result->objectDef.vars[varCount] = p->variableDef;
		vars = vars->oper.op[0];
	}

	return result;
}

Node* object(const std::string &name) {
	Node* result = newNode();
	result->type = typeObject;
	result->object.name = name;
	return result;
}


Node* variableDef(Node* typedVariable, Node* defaultValue) {
	Node* result = newNode();
	result->type = typeVariableDef;

	VariableDefNode* variableDef = &result->variableDef;

	variableDef->name = typedVariable->variable.name;
	variableDef->value.dataType = typedVariable->variable.dataType;
	variableDef->defaultValue = defaultValue;

	return result;
}

Node* typedVariable(std::string name, dataTypeEnum type) {
	VariableNode variableNode;
	variableNode.dataType = type;
	variableNode.name = std::move(name);

	Node* variable = newNode();
	variable->type = typeVariable;
	variable->variable = variableNode;
//	variable->variable.dataType = type;
//	variable->variable.name = name;
	return variable;
}

Node* variable(std::string name) {
	return typedVariable(std::move(name), typeUndefined);
}


Node* empty() {
	auto* empty = newNode();
	empty->type = typeEmpty;
	return empty;
}


Node* op(int oper, int opCount, ...) {
	/* allocate node, extending op array */
	Node* operand = (Node*) malloc(sizeof(Node) + opCount * sizeof(Node*));

	operand->type = typeOperator;
	operand->oper.oper = oper;
	operand->oper.opCount = opCount;

	va_list ap;
	va_start(ap, opCount);
	for (int i = 0; i < opCount; i++) {
		operand->oper.op[i] = va_arg(ap, Node*);
	}
	va_end(ap);

	return operand;
}

#endif //CRACKLE_INTERPRETER_PARSER_H
