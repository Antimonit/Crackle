#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "symbol_table.h"
#include "types.h"
#include "debug.h"


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


void printAllSymbols() {
	symbolTable* table = currentSymbolTable;
	printf("----ALL SYMBOLS----\n");
	for (int i = 0; i < table->symbolId; ++i) {
		if (table->variables[i].type == typeInt) {
			printf("Int %s: %d\n", table->symbols[i], table->variables[i].intVal);
		} else if (table->variables[i].type == typeDouble) {
			printf("Double %s: %f\n", table->symbols[i], table->variables[i].doubleVal);
		} else if (table->variables[i].type == typeString) {
			printf("String %s: %s\n", table->symbols[i], table->variables[i].stringVal);
		} else if (table->variables[i].type == typeBool) {
			printf("Bool %s: %s\n", table->symbols[i], table->variables[i].boolVal ? "true" : "false");
		}
	}
	printf("----END ALL SYMBOLS----\n");
}


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

constantNode* addSymbolNode(variableNode variableNode, dataTypeEnum type) {
	symbolTable *table = currentSymbolTable;
	constantNode* variables = table->variables;

	constantNode* symbolNode = findSymbolNodeInTable(table, variableNode.name);
	if (symbolNode == NULL) {	// new symbol
		strncpy(table->symbols[table->symbolId], variableNode.name, strlen(variableNode.name));
		constantNode* variable = &variables[table->symbolId];
		table->symbolId++;
		variable->type = type;
		return variable;
	} else {			// existing symbol
		printf("Warning: Trying to redeclare variable '%s' of type %s to type %s.\n",
			  variableNode.name,
              getDataTypeString(variables->type),
              getDataTypeString(type));

//		if (dataType == typeDouble && symbolTypes[index] == typeInt) {
//			debug("\tChange variableNode from int to double\n");
//			doubleVariables[index] = (double) intVariables[index];
//			symbolTypes[index] = typeDouble;
//		} else if (dataType == typeInt && symbolTypes[index] == typeDouble) {
//			debug("\tChange variableNode from double to int\n");
//			intVariables[index] = (int) doubleVariables[index];
//			symbolTypes[index] = typeInt;
//		}
		return NULL;
	}
}
