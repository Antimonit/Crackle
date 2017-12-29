#ifndef SYMBOLS_H_
#define SYMBOLS_H_

#include <stdlib.h>
#include "types.h"

#define MAX_VARS 100
#define MAX_VAR_NAME_LENGTH 16

typedef struct symbolTable {
    char symbols[MAX_VARS][MAX_VAR_NAME_LENGTH];
    constantNode variables[MAX_VARS];
    struct symbolTable* parentTable;
} symbolTable;

char symbols[MAX_VARS][MAX_VAR_NAME_LENGTH];
constantNode variables[MAX_VARS];


typedef struct {
    char funcSymbols[MAX_VARS][MAX_VAR_NAME_LENGTH];
    node* funcRoots[MAX_VARS];
} functionSymbolTable;

char funcSymbols[MAX_VARS][MAX_VAR_NAME_LENGTH];
node* funcRoots[MAX_VARS];


symbolTable globalSymbolTable;
functionSymbolTable functionTable;

symbolTable* currentSymbolTable;

int findSymbol(const char* symbol);

int findTypedSymbolNode(variableNode variable, dataTypeEnum type);

int addSymbolNode(variableNode variable, dataTypeEnum type);

void printAllSymbols();

int findFunctionSymbol(const char* symbol);

int addFuncRoot(const char* symbol, node* root);

#endif
