#ifndef CRACKLE_INTERPRETER_NODE_HELPERS_H
#define CRACKLE_INTERPRETER_NODE_HELPERS_H

#include "types.hpp"

#ifdef __cplusplus
extern "C" {
#endif

dataTypeEnum widenNodes(constantNode* a, constantNode* b);

void defaultConstant(constantNode* def);

#ifdef __cplusplus
}
#endif

#endif //CRACKLE_INTERPRETER_NODE_HELPERS_H
