#ifndef __NODE_HELPERS_HPP__
#define __NODE_HELPERS_HPP__

#include "types.hpp"

DataType maxType(ConstantNode& a, ConstantNode& b);

DataType widenNodes(ConstantNode& a, ConstantNode& b);

void defaultConstant(ConstantNode& def);

#endif //CRACKLE_INTERPRETER_NODE_HELPERS_H
