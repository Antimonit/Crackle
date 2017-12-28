#ifndef SYMBOLS_H_
#define SYMBOLS_H_

#include "types.h"

#define MAX_VARS 100
#define MAX_VAR_NAME_LENGTH 16

char symbols[MAX_VARS][MAX_VAR_NAME_LENGTH];
typeEnum symbolTypes[MAX_VARS];
int intVariables[MAX_VARS];
double doubleVariables[MAX_VARS];
char* stringVariables[MAX_VARS];

char funcSymbols[MAX_VARS][MAX_VAR_NAME_LENGTH];
node* funcRoots[MAX_VARS];


int findSymbol(const char* symbol);

int addSymbolNode(variableNode variable, typeEnum type);

void printAllSymbols();

int findFunctionSymbol(const char* symbol);

int addFuncRoot(const char* symbol, node* root);

#endif