//
// Created by DAVE on 7. 2. 2018.
//

#include "xFunctionDefNode.h"
#include "Driver.hpp"

#include "xEmptyNode.h"


xNode* xFunctionDefNode::ex(MC::Driver* driver) {
	driver->addFunction(this);
	return new xEmptyNode;
}

std::string xFunctionDefNode::print() const {
	return "Function definition";
}

std::ostream& xFunctionDefNode::print(std::ostream& out) const {
	return out << name << " "
			   << params.size() << " params";
}
