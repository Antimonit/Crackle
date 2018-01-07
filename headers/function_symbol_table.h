#ifndef FUNCTION_SYMBOL_TABLE_H
#define FUNCTION_SYMBOL_TABLE_H

#include <stdlib.h>
#include "types.h"

#define MAX_VARS 100
#define MAX_VAR_NAME_LENGTH 16

typedef struct {
	int symbolId;
	functionNode* functions[MAX_VARS];
} functionSymbolTable;

functionNode* findFunction(const char *symbol);

void addFunction(functionNode* function);

#endif
