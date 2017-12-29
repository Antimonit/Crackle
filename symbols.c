#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "symbols.h"
#include "types.h"
#include "debug.h"

int symbolId = 0;
int funcSymbolId = 0;

void printAllSymbols() {
	printf("----ALL SYMBOLS----\n");
	for (int i = 0; i < symbolId; ++i) {
		if (variables[i].type == typeInt) {
			printf("Int %s: %d\n", symbols[i], variables[i].intVal);
		} else if (variables[i].type == typeDouble) {
			printf("Double %s: %f\n", symbols[i], variables[i].doubleVal);
		} else if (variables[i].type == typeString) {
			printf("String %s: %s\n", symbols[i], variables[i].stringVal);
		} else if (variables[i].type == typeBool) {
			printf("Bool %s: %s\n", symbols[i], variables[i].boolVal ? "true" : "false");
		}
	}
	printf("----END ALL SYMBOLS----\n");
}

symbolTable* addSymbolTableScope() {
    symbolTable* table = malloc(sizeof(symbolTable));
    table->parentTable = currentSymbolTable;
    currentSymbolTable = table;
    return table;
}


int findSymbol(const char* symbol) {
    for (int i = 0; i < symbolId; i++) {
        if (strcmp(symbols[i], symbol) == 0) {
            return i;
        }
    }
    return -1;
}

int findTypedSymbolNode(variableNode variable, dataTypeEnum type) {
    int index = findSymbol(variable.name);
    if (index != -1 && variables[index].type == type) {
        return index;
    }
    return -1;
}

int addSymbolNode(variableNode variable, dataTypeEnum type) {
	int index = findSymbol(variable.name);
	if (index == -1) {	// new symbol
		strncpy(symbols[symbolId], variable.name, strlen(variable.name));
		variables[symbolId].type = type;
		return symbolId++;
	} else {			// existing symbol
		debug("Variable %s already defined\n", variable.name);

//		if (dataType == typeDouble && symbolTypes[index] == typeInt) {
//			debug("\tChange variable from int to double\n");
//			doubleVariables[index] = (double) intVariables[index];
//			symbolTypes[index] = typeDouble;
//		} else if (dataType == typeInt && symbolTypes[index] == typeDouble) {
//			debug("\tChange variable from double to int\n");
//			intVariables[index] = (int) doubleVariables[index];
//			symbolTypes[index] = typeInt;
//		}
		return index;
	}
}


int findFunctionSymbol(const char* symbol) {
    for (int i = 0; i < funcSymbolId; i++) {
        if (strcmp(funcSymbols[i], symbol) == 0) {
            return i;
        }
    }
    return -1;
}

int addFuncRoot(const char* symbol, node* root) {
	int index = findSymbol(symbol);
	if (index == -1) {	// new symbol
		strncpy(funcSymbols[funcSymbolId], symbol, strlen(symbol));
		funcRoots[funcSymbolId] = root;
		return funcSymbolId++;
	} else {
		printf("Note: redefining function %s.\n", symbol);
		funcRoots[index] = root;
		return index;
	}
}
