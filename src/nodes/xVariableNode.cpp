//
// Created by DAVE on 7. 2. 2018.
//

#include "Driver.hpp"
#include "xVariableNode.h"
#include "xEmptyNode.h"
#include "DataType.h"
#include "xConstantNode.h"

xVariableNode::xVariableNode() {
	name = "";
	value = new xConstantNode(typeUndefined);
}

xNode* xVariableNode::ex(MC::Driver* driver) {
	xVariableNode* variable = driver->findVariable(name);

	if (variable == nullptr) {
		std::cerr << "Warning: Undefined variable '" << name << "'." << std::endl;
		xEmptyNode* empty = new xEmptyNode;
		return empty;
	}

	return variable;
}

std::string xVariableNode::print() const {
	return "Variable node";
}

std::ostream& xVariableNode::print(std::ostream& out) const {
	return out << *value;
}
