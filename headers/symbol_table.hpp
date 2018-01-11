#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdlib.h>
#include "types.hpp"

#ifdef __cplusplus
extern "C" {
#endif

void replaceSymbolTableScope();

void pushSymbolTableScope();

void popSymbolTableScope();

constantNode* findVariable(const char *symbol);

void addVariable(variableDefNode* variable);

functionDefNode* findFunction(const char *symbol);

void addFunction(functionDefNode* function);

#ifdef __cplusplus
}
#endif

#endif
