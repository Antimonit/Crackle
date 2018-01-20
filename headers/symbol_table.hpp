#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdlib.h>
#include "types.hpp"

void replaceSymbolTableScope();

void pushSymbolTableScope();

void popSymbolTableScope();

ConstantNode* findVariable(const std::string &symbol);
void addVariable(VariableDefNode* variable);

FunctionDefNode* findFunction(const std::string &symbol);
void addFunction(FunctionDefNode* function);

ObjectDefNode* findObject(const std::string &symbol);
void addObject(ObjectDefNode* object);


#endif
