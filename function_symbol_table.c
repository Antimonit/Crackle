#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "function_symbol_table.h"
#include "types.h"
#include "debug.h"

functionSymbolTable functionTable;


functionRecord* findFunction(const char *symbol) {
	functionSymbolTable* table = &functionTable;
	for (int i = 0; i < table->symbolId; i++) {
		functionRecord* function = &table->functions[i];
		if (strcmp(function->name, symbol) == 0) {
			return function;
		}
	}
	return NULL;
}

node* findFunctionRoot(const char *symbol) {
	functionRecord* function = findFunction(symbol);
	if (function == NULL)
		return NULL;
	else
		return function->rootNode;
}

void addFunctionRoot(const char *symbol, node *root, dataTypeEnum returnType) {
	if (findFunction(symbol) != NULL) {
		debug("Trying to redeclare function '%s'\n", symbol);
		return;
	}
	// new symbol
	functionSymbolTable* table = &functionTable;
	functionRecord* function = &table->functions[table->symbolId];
	strncpy(function->name, symbol, strlen(symbol));
	function->rootNode = root;
	function->returnType = returnType;
	table->symbolId++;
}
