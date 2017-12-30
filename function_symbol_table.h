#ifndef FUNCTION_SYMBOL_TABLE_H
#define FUNCTION_SYMBOL_TABLE_H

#include <stdlib.h>
#include "types.h"

node* findFunctionRoot(const char *symbol);

node* addFunctionRoot(const char *symbol, node *root);

#endif
