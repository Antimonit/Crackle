//
// Created by DAVE on 7. 2. 2018.
//

#include "xVariableDefNode.h"
#include "Driver.hpp"
#include "xConstantNode.h"
#include "xVariableNode.h"
#include "xEmptyNode.h"

xVariableDefNode::xVariableDefNode(
		const std::string& name,
		xConstantNode* value
) : name(name), value(value) {}

xVariableDefNode::xVariableDefNode(
		const std::string& name,
		xConstantNode* value,
		xNode* defaultValue
) : name(name), value(value), defaultValue(defaultValue) {}


xNode* xVariableDefNode::ex(MC::Driver* driver) {
	if (defaultValue != nullptr) {
		xConstantNode* value = defaultValue->ex(driver)->getValue();

		if (this->value->getType() != value->getType()) {
			std::cerr << "Warning: Defined value of type " << this->value->getType()
					  << " is incompatible with variable type " << value->getType()
					  << "." << std::endl;
		} else {
			this->value = value;
		}
	} else {
		this->value->defaultValue();
	}

	auto* variable = new xVariableNode(name, value);
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
	return out << value->getType() << " " << name;
}
