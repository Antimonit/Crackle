//
// Created by DAVE on 7. 2. 2018.
//

#ifndef CRACKLE_XFUNCTIONDEFNODE_H
#define CRACKLE_XFUNCTIONDEFNODE_H

#include <c++/string>
#include <c++/vector>
#include "Driver.hpp"
#include "node_types.h"
#include "xNode.h"
#include "xEmptyNode.h"

class xFunctionDefNode : private xNode {
public:
	std::string name;
	DataType dataType;
	xNode* root;
	std::vector<xVariableDefNode*> params;	// formal parameters

	xNode* ex(MC::Driver* driver) override {
		driver->addFunction(this);
		return new xEmptyNode;
	}
};


#endif //CRACKLE_XFUNCTIONDEFNODE_H
