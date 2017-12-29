#ifndef FUNCTION_SYMBOL_TABLE_H
#define FUNCTION_SYMBOL_TABLE_H

#include <stdlib.h>
#include "types.h"

#define MAX_VARS 100
#define MAX_VAR_NAME_LENGTH 16

typedef struct {
    int symbolId;
    char symbols[MAX_VARS][MAX_VAR_NAME_LENGTH];
    node* roots[MAX_VARS];
} functionSymbolTable;


functionSymbolTable functionTable;


node* findFunctionRoot(const char *symbol);

node* addFunctionRoot(const char *symbol, node *root);


#endif
