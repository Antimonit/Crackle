//
// Created by DAVE on 7. 2. 2018.
//

#include "xVariableDefNode.h"
#include "Driver.hpp"
#include "xConstantNode.h"
#include "xVariableNode.h"
#include "xEmptyNode.h"

xVariableDefNode::xVariableDefNode() {
	name = "";
	value = new xConstantNode(typeUndefined);
}

xNode* xVariableDefNode::ex(MC::Driver* driver) {

	xConstantNode* varDefValue = value;
	if (defaultValue != nullptr) {
		xNode* defValue = defaultValue->ex(driver);
		xConstantNode* value = defValue->getValue();

		if (varDefValue->getType() != value->getType()) {
			std::cerr << "Warning: Defined value of type " << varDefValue->getType()
					  << " is incompatible with variable type " << value->getType()
					  << "." << std::endl;
		} else {
			varDefValue = value;	// TODO: check this
		}
	} else {
		varDefValue->defaultValue();
	}

	auto* variable = new xVariableNode;
	variable->name = name;
	variable->value = value;

	driver->addVariable(variable);

	return new xEmptyNode();
}

xConstantNode* xVariableDefNode::getValue() {
	return value;
}

std::string xVariableDefNode::print() const {
	return "Variable definition";
}

std::ostream& xVariableDefNode::print(std::ostream& out) const {
	return out << name;
}
