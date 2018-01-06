#ifndef FUNCTION_SYMBOL_TABLE_H
#define FUNCTION_SYMBOL_TABLE_H

#include <stdlib.h>
#include "types.h"

#define MAX_VARS 100
#define MAX_VAR_NAME_LENGTH 16

typedef struct {
	char name[MAX_VAR_NAME_LENGTH];
	node* rootNode;
	dataTypeEnum returnType;
	int paramCount;
	variableNode* params;
} functionRecord;

typedef struct {
	int symbolId;
	functionRecord functions[MAX_VARS];
} functionSymbolTable;


functionRecord* findFunction(const char *symbol);

void addFunction(const char *symbol, node *root, dataTypeEnum returnType);

#endif
