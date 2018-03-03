//
// Created by DAVE on 7. 2. 2018.
//

#include "xVariableDefNode.h"
#include "xVariableNode.h"
#include "xEmptyNode.h"

xNode* xVariableDefNode::ex() {
	xVariableDefNode& variableDef = *this;

	xConstantNode& varDefValue = value;
	if (defaultValue != nullptr) {
		xNode* defVal = defaultValue->ex();
		xConstantNode& value = (defVal->getType() == Node::Variable) ? defVal->variable->value
																	 : defVal->constant;

		if (varDefValue.getType() != value.getType()) {
			std::cerr << "Warning: Defined value of type " << varDefValue.getType()
					  << " is incompatible with variable type " << value.getType()
					  << "." << std::endl;
		} else {
			varDefValue = value;
		}
	} else {
		varDefValue.defaultValue();
	}

	auto* variable = new xVariableNode();
	variable->name = name;
	variable->value = value;

	addVariable(variable);

	return new xEmptyNode();
}
