#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "function_symbol_table.h"
#include "types.h"
#include "debug.h"

functionSymbolTable functionTable;

node* findFunctionRoot(const char *symbol) {
	functionSymbolTable* table = &functionTable;
    for (int i = 0; i < table->symbolId; i++) {
        if (strcmp(table->symbols[i], symbol) == 0) {
            return table->roots[i];
        }
    }
    return NULL;
}

node* addFunctionRoot(const char *symbol, node *root) {

	functionSymbolTable* table = &functionTable;
	node* currentRoot = findFunctionRoot(symbol);
	if (currentRoot == NULL) { // new symbol
		strncpy(table->symbols[table->symbolId], symbol, strlen(symbol));
		table->roots[table->symbolId] = root;
		table->symbolId++;
		return root;
	} else {
		debug("Trying to redeclare function '%s'\n", symbol);
		return NULL;
	}

//	int index = findSymbolNode(symbol);
//	if (index == -1) {	// new symbol
//		strncpy(table->symbols[table->symbolId], symbol, strlen(symbol));
//		table->roots[table->symbolId] = root;
//		return table->symbolId++;
//	} else {
//		printf("Note: redefining function %s.\n", symbol);
//		table->roots[index] = root;
//		return index;
//	}
}
