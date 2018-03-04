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
#include <nodes/xNode.h>
#include <nodes/xOperatorNode.h>
#include <nodes/xObjectNode.h>
#include <nodes/xObjectDefNode.h>
#include <nodes/xEmptyNode.h>
#include <nodes/xVariableNode.h>
#include <nodes/xVariableDefNode.h>
#include <nodes/xFunctionNode.h>
#include <nodes/xFunctionDefNode.h>
#include <nodes/xConstantNode.h>
#include "headers/types.hpp"
#include "headers/SymbolTable.hpp"


int yyerror(const std::string &message) {
	std::cerr << message << std::endl;
	return 1;
}


//void deleteNode(xNode* node) {
//	if (!node)
//		return;
//
//	if (node->getType() == Node::Operator) {
//		for (auto& i : node->oper.op) {
//			deleteNode(i);
//		}
//	}
//	free(node);
//}


int getParamCount(xOperatorNode* params) {
	int paramCount = 0;
	if (params != nullptr) {
		paramCount = 1;
		xOperatorNode* countingParams = params;
		while (countingParams->op[0] != nullptr) {
			paramCount++;
			countingParams = dynamic_cast<xOperatorNode*>(countingParams->op[0]);
		}
	}
	return paramCount;
}

xFunctionDefNode* functionDef(xVariableNode* typedVariable, xOperatorNode* params, xNode* root) {
	auto* functionDef = new xFunctionDefNode;

	xVariableNode* variable = typedVariable;

	functionDef->dataType = variable->value->getType();
	functionDef->name = variable->name;
	functionDef->root = root;

	/* retrieve actual parameters */
	int paramCount = getParamCount(params);
	while (paramCount > 0) {
		paramCount--;
		xVariableNode* var = dynamic_cast<xVariableNode*>(params->op[1]);
		xVariableDefNode* variableDef = new xVariableDefNode;
		variableDef->name = var->name;
		variableDef->value->setType(var->value->getType());
		functionDef->params.push_back(variableDef);
		params = dynamic_cast<xOperatorNode*>(params->op[0]);
	}

	return functionDef;
}

xFunctionNode* function(const std::string& name, xOperatorNode* params) {
	auto* function = new xFunctionNode;
	function->name = name;

	/* retrieve actual parameters */
	int paramCount = getParamCount(params);
	while (paramCount > 0) {
		paramCount--;
		function->params.push_back(params->op[1]);
		params = dynamic_cast<xOperatorNode*>(params->op[0]);
	}

	return function;
}


xConstantNode* constantInt(int value) {
	return new xConstantNode(value);
}

xConstantNode* constantDouble(double value) {
	return new xConstantNode(value);
}

xConstantNode* constantString(const std::string &value) {
	return new xConstantNode(value);
}

xConstantNode* constantBool(bool value) {
	return new xConstantNode(value);
}

xConstantNode* constantNull() {
	return new xConstantNode(nullptr, "null");
}


xObjectDefNode* objectDef(const std::string &name, xOperatorNode* vars) {
	/* count number of vars */
	int varCount = getParamCount(vars);

	/* allocate node, extending params array */
	auto* node = new xObjectDefNode();
	node->name = name;
	while (varCount > 0) {
		varCount--;
		xVariableDefNode* p = dynamic_cast<xVariableDefNode*>(vars->op[1]); // p is variableDef
		node->vars.push_back(p);
		vars = dynamic_cast<xOperatorNode*>(vars->op[0]);
	}

	return node;
}

xObjectNode* object(const std::string &name) {
	auto* node = new xObjectNode;
	node->name = name;
	return node;
}


xVariableDefNode* variableDef(xVariableNode* typedVariable, xNode* defaultValue) {
	auto* result = new xVariableDefNode();
	result->name = typedVariable->name;
	result->value->setType(typedVariable->value->getType());
	result->defaultValue = defaultValue;
	return result;
}

xVariableNode* typedVariable(const std::string& name, DataType type) {
	auto* variableNode = new xVariableNode();
	variableNode->name = name;
	variableNode->value->setType(type);
	return variableNode;
}

xVariableNode* variable(const std::string& name) {
	return typedVariable(name, typeUndefined);
}


xEmptyNode* empty() {
	return new xEmptyNode;
}

xOperatorNode* op(int oper, int opCount, ...) {
	auto* operatorNode = new xOperatorNode(oper);
	operatorNode->oper = oper;
	operatorNode->op = std::vector<xNode*>(static_cast<unsigned long long int>(opCount));

	va_list ap;
	va_start(ap, opCount);
	for (int i = 0; i < opCount; i++) {
		operatorNode->op[i] = va_arg(ap, xNode*);
	}
	va_end(ap);
	return operatorNode;
}

#endif //CRACKLE_INTERPRETER_PARSER_H
