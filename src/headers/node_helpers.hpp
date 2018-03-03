#ifndef __NODE_HELPERS_HPP__
#define __NODE_HELPERS_HPP__

#include "types.hpp"

DataType maxType(xConstantNode& a, xConstantNode& b);

DataType widenNodes(xConstantNode& a, xConstantNode& b);

void defaultConstant(xConstantNode& def);

#endif //CRACKLE_INTERPRETER_NODE_HELPERS_H
