#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "headers/function_symbol_table.h"
#include "headers/types.h"
#include "headers/debug.h"

functionSymbolTable functionTable;


functionNode* findFunction(const char *symbol) {
	for (int i = 0; i < functionTable.symbolId; i++) {
		functionNode* function = functionTable.functions[i];
		if (strcmp(function->name, symbol) == 0) {
			return function;
		}
	}
	return NULL;
}

void addFunction(functionNode* function) {
	if (findFunction(function->name) != NULL) {
		printf("Warning: Trying to redeclare function '%s'\n", function->name);
		return;
	}
	// new symbol
	functionTable.functions[functionTable.symbolId] = function;
	functionTable.symbolId++;
}