#ifndef __PARSER_HPP__
#define __PARSER_HPP__

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
#include "headers/SymbolTable.hpp"


int yyerror(const std::string &message) {
	std::cerr << message << std::endl;
	return 1;
}


void deleteNode(Node* node) {
	if (!node)
		return;

	if (node->getType() == Node::Operator) {
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

	VariableNode* variable = typedVariable->variable;
	FunctionDefNode* functionDef = &function->functionDef;

	function->setType(Node::FunctionDef);
	functionDef->dataType = variable->value.getType();
	functionDef->name = variable->name;
	functionDef->root = root;

	/* retrieve actual parameters */
	int paramCount = getParamCount(params);
	while (paramCount > 0) {
		paramCount--;
		VariableNode* var = params->oper.op[1]->variable;
		VariableDefNode variableDef = *new VariableDefNode;
		variableDef.name = var->name;
		variableDef.value.setType(var->value.getType());
		functionDef->params.push_back(variableDef);
		params = params->oper.op[0];
	}

	return function;
}

Node* function(const std::string& name, Node* params) {
	auto* functionCall = new Node;
	functionCall->setType(Node::Function);
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
	constant->setType(Node::Constant);
	constant->constant.setType(typeInt);
	constant->constant.intVal = value;
	return constant;
}

Node* constantDouble(double value) {
	auto* constant = new Node();
	constant->setType(Node::Constant);
	constant->constant.setType(typeDouble);
	constant->constant.doubleVal = value;
	return constant;
}

Node* constantString(const std::string &value) {
	auto* constant = new Node();
	constant->setType(Node::Constant);
	constant->constant.setType(typeString);
	constant->constant.stringVal = value;
	return constant;
}

Node* constantBool(bool value) {
	auto* constant = new Node();
	constant->setType(Node::Constant);
	constant->constant.setType(typeBool);
	constant->constant.boolVal = value;
	return constant;
}

Node* constantNull() {
	auto* constant = new Node();
	constant->setType(Node::Constant);
	constant->constant.setType(typeObject);
	constant->constant.objectVal = nullptr;
	constant->constant.objectTypeName = "null";
	return constant;
}


Node* objectDef(const std::string &name, Node* vars) {
	/* count number of vars */
	int varCount = getParamCount(vars);

	/* allocate node, extending params array */
	auto* result = new Node;
	result->setType(Node::ObjectDef);
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
	result->setType(Node::Object);
	result->object.name = name;
	return result;
}


Node* variableDef(Node* typedVariable, Node* defaultValue) {
	auto* result = new Node();
	result->setType(Node::VariableDef);

	VariableDefNode* variableDef = &result->variableDef;

	variableDef->name = typedVariable->variable->name;
	variableDef->value.setType(typedVariable->variable->value.getType());
	variableDef->defaultValue = defaultValue;

	return result;
}

Node* typedVariable(const std::string& name, DataType type) {
	auto* variable = new Node();
	variable->setType(Node::Variable);
	variable->variable = new VariableNode;
	variable->variable->name = name;
	variable->variable->value.setType(type);
	return variable;
}

Node* variable(const std::string& name) {
	return typedVariable(name, typeUndefined);
}


Node* empty() {
	auto* empty = new Node();
	empty->setType(Node::Empty);
	return empty;
}

Node* op(int oper, int opCount, ...) {
	auto* operand = new Node;

	operand->setType(Node::Operator);
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
