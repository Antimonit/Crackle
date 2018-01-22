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


void deleteNode(Node* node) {
	if (!node)
		return;

	if (node->getType() == typeOperator) {
		for (auto& i : node->oper.op) {
			deleteNode(i);
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

Node* functionDef(Node* typedVariable, Node* params, Node* root) {
	auto* function = new Node;

	VariableNode variable = typedVariable->variable;
	FunctionDefNode* functionDef = &function->functionDef;

	function->setType(typeFunctionDef);
	functionDef->dataType = variable.dataType;
	functionDef->name = variable.name;
	functionDef->root = root;

	/* retrieve actual parameters */
	int paramCount = getParamCount(params);
	while (paramCount > 0) {
		paramCount--;
		VariableNode& var = params->oper.op[1]->variable;
		VariableDefNode variableDef;
		variableDef.name = var.name;
		variableDef.value.setType(var.dataType);
		functionDef->params.push_back(variableDef);
		params = params->oper.op[0];
	}

	return function;
}

Node* function(const std::string& name, Node* params) {
	auto* functionCall = new Node;
	functionCall->setType(typeFunction);
	functionCall->function.name = name;

	/* retrieve actual parameters */
	int paramCount = getParamCount(params);
	while (paramCount > 0) {
		paramCount--;
		functionCall->function.params.push_back(params->oper.op[1]);
		params = params->oper.op[0];
	}

	return functionCall;
}


Node* constantInt(int value) {
	auto* constant = new Node();
	constant->setType(typeConstant);
	constant->constant.setType(typeInt);
	constant->constant.intVal = value;
	return constant;
}

Node* constantDouble(double value) {
	auto* constant = new Node();
	constant->setType(typeConstant);
	constant->constant.setType(typeDouble);
	constant->constant.doubleVal = value;
	return constant;
}

Node* constantString(const std::string &value) {
	auto* constant = new Node();
	constant->setType(typeConstant);
	constant->constant.setType(typeString);
	constant->constant.stringVal = value;
	return constant;
}

Node* constantBool(bool value) {
	auto* constant = new Node();
	constant->setType(typeConstant);
	constant->constant.setType(typeBool);
	constant->constant.boolVal = value;
	return constant;
}

Node* constantNull() {
	auto* constant = new Node();
	constant->setType(typeConstant);
	constant->constant.setType(typeObj);
	constant->constant.objectVal = nullptr;
	constant->constant.objectTypeName = "null";
	return constant;
}


Node* objectDef(const std::string &name, Node* vars) {
	/* count number of vars */
	int varCount = getParamCount(vars);

	/* allocate node, extending params array */
	auto* result = new Node;
	result->setType(typeObjectDef);
	result->objectDef.name = name;
	while (varCount > 0) {
		varCount--;
		Node *p = vars->oper.op[1]; // p is variableDef
		result->objectDef.vars.push_back(p->variableDef);
		vars = vars->oper.op[0];
	}

	return result;
}

Node* object(const std::string &name) {
	auto* result = new Node();
	result->setType(typeObject);
	result->object.name = name;
	return result;
}


Node* variableDef(Node* typedVariable, Node* defaultValue) {
	auto* result = new Node();
	result->setType(typeVariableDef);

	VariableDefNode* variableDef = &result->variableDef;

	variableDef->name = typedVariable->variable.name;
	variableDef->value.setType(typedVariable->variable.dataType);
	variableDef->defaultValue = defaultValue;

	return result;
}

Node* typedVariable(const std::string& name, DataType type) {
	auto* variable = new Node();
	variable->setType(typeVariable);
	variable->variable.dataType = type;
	variable->variable.name = name;
	return variable;
}

Node* variable(const std::string& name) {
	return typedVariable(name, typeUndefined);
}


Node* empty() {
	auto* empty = new Node();
	empty->setType(typeEmpty);
	return empty;
}

Node* op(int oper, int opCount, ...) {
	auto* operand = new Node;

	operand->setType(typeOperator);
	operand->oper.oper = oper;
	operand->oper.op = std::vector<Node*>(static_cast<unsigned long long int>(opCount));

	va_list ap;
	va_start(ap, opCount);
	for (int i = 0; i < opCount; i++) {
		operand->oper.op[i] = va_arg(ap, Node*);
	}
	va_end(ap);

	return operand;
}

#endif //CRACKLE_INTERPRETER_PARSER_H
