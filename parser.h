//
// Created by DAVE on 31. 12. 2017.
//

#ifndef CRACKLE_INTERPRETER_PARSER_H
#define CRACKLE_INTERPRETER_PARSER_H

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include "types.h"
#include "interpreter.h"
#include "symbol_table.h"

int yyerror(const char* message) {
	printf(message);
}


node* newNode() {
	node* p = (node*) malloc(sizeof(node));

	if (p == NULL)
		yyerror("out of memory");

	return p;
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


node* empty() {
	node* node = newNode();

	node->type = typeEmpty;

	return node;
}

node* funCall(const char* value, node* params) {
	node* node = newNode();

	node->type = typeFunctionCall;
	node->functionCall.name = value;

	return node;
}

node* constantInt(int value) {
	node* node = newNode();

	node->type = typeConstant;
	node->constant.type = typeInt;
	node->constant.intVal = value;

	return node;
}

node* constantDouble(double value) {
	node* node = newNode();

	node->type = typeConstant;
	node->constant.type = typeDouble;
	node->constant.doubleVal = value;

	return node;
}

node* constantString(char* value) {
	node* node = newNode();

	node->type = typeConstant;
	node->constant.type = typeString;
	node->constant.stringVal = value;

	return node;
}

node* constantBool(bool value) {
	node* node = newNode();

	node->type = typeConstant;
	node->constant.type = typeBool;
	node->constant.boolVal = value;

	return node;
}

node* typedVariable(const char* name, dataTypeEnum type) {
	node* node = newNode();

	node->type = typeVariable;
	node->variable.dataType = type;
	node->variable.name = name;

	return node;
}

node* variable(const char* name) {
	node* node = newNode();

	node->type = typeVariable;
//	node->variable.dataType = UNSPECIFIED;
	node->variable.name = name;

	return node;
}

node* op(int oper, int opCount, ...) {
	/* allocate node, extending op array */
	node* p = malloc(sizeof(node) + (opCount-1) * sizeof(node*));

	if (p == NULL)
		yyerror("out of memory");

	p->type = typeOperator;
	p->oper.oper = oper;
	p->oper.opCount = opCount;

	va_list ap;
	va_start(ap, opCount);
	for (int i = 0; i < opCount; i++) {
		p->oper.op[i] = va_arg(ap, node*);
	}
	va_end(ap);

	return p;
}

#endif //CRACKLE_INTERPRETER_PARSER_H
