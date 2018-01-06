#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdlib.h>
#include "types.h"

void pushSymbolTableScope();

void popSymbolTableScope();

constantNode* findSymbolNode(const char *symbol);

constantNode* addSymbolNode(variableNode variableNode, dataTypeEnum type);

#endif
