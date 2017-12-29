#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdlib.h>
#include "types.h"

#define MAX_VARS 100
#define MAX_VAR_NAME_LENGTH 16

typedef struct symbolTable {
    int symbolId;
    char symbols[MAX_VARS][MAX_VAR_NAME_LENGTH];
    constantNode variables[MAX_VARS];
    struct symbolTable* parentTable;
} symbolTable;


symbolTable globalSymbolTable;

symbolTable* currentSymbolTable;


constantNode* findSymbolNode(const char *symbol);

constantNode* findTypedSymbolNode(variableNode variableNode, dataTypeEnum type);

constantNode* addSymbolNode(variableNode variableNode, dataTypeEnum type);


#endif
