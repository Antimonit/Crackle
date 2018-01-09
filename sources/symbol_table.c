#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "headers/symbol_table.h"
#include "headers/types.h"
#include "headers/debug.h"


#define MAX_VARS 100
#define MAX_VAR_NAME_LENGTH 16

typedef struct symbolTable {
	int symbolId;
	char symbols[MAX_VARS][MAX_VAR_NAME_LENGTH];
	constantNode variables[MAX_VARS];
	struct symbolTable* parentTable;
} symbolTable;


symbolTable globalSymbolTable;

symbolTable* currentSymbolTable = &globalSymbolTable;


void pushSymbolTableScope() {
    symbolTable* table = malloc(sizeof(symbolTable));
	table->symbolId = 0;
    table->parentTable = currentSymbolTable;
    currentSymbolTable = table;
}

void popSymbolTableScope() {
	symbolTable* table = currentSymbolTable;
	currentSymbolTable = table->parentTable;
	free(table);
}


constantNode* findSymbolNodeInTable(symbolTable* table, const char *symbol) {
    for (int i = 0; i < table->symbolId; i++) {
        if (strcmp(table->symbols[i], symbol) == 0) {
            return &table->variables[i];
        }
    }
    return NULL;
}

constantNode* findSymbolNode(const char *symbol) {
	symbolTable* table = currentSymbolTable;
	while (table != NULL) {
        constantNode *res = findSymbolNodeInTable(table, symbol);
        if (res != NULL) {
            return res;
        }
		table = table->parentTable;
	}
    return NULL;
}

constantNode* addSymbolNode(variableNode variableNode) {
	symbolTable *table = currentSymbolTable;
	constantNode* variables = table->variables;

	const char *name = variableNode.name;
	dataTypeEnum dataType = variableNode.dataType;

	constantNode* symbolNode = findSymbolNodeInTable(table, name);
	if (symbolNode == NULL) {	// new symbol
		strncpy(table->symbols[table->symbolId], name, strlen(name));
		constantNode* variable = &variables[table->symbolId];
		table->symbolId++;
		variable->dataType = dataType;
		return variable;
	} else {			// existing symbol
		printf("Warning: Trying to redeclare variable '%s' of type %s to type %s.\n",
			   name,
			   dataTypeToString(variables->dataType),
			   dataTypeToString(dataType));
		return NULL;
	}
}
