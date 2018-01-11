#ifndef CRACKLE_INTERPRETER_NODE_HELPERS_H
#define CRACKLE_INTERPRETER_NODE_HELPERS_H

#include "types.hpp"

#ifdef __cplusplus
extern "C" {
#endif

dataTypeEnum widenNodes(constantNode* a, constantNode* b);

void copyConstantToVariableDef(variableDefNode* to, constantNode* from);

void copyVariableDefToConstant(constantNode* to, variableDefNode* from);

void defaultVariableDef(variableDefNode* def);

#ifdef __cplusplus
}
#endif

#endif //CRACKLE_INTERPRETER_NODE_HELPERS_H
