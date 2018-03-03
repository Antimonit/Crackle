//
// Created by DAVE on 7. 2. 2018.
//

#ifndef CRACKLE_XVARIABLEDEFNODE_H
#define CRACKLE_XVARIABLEDEFNODE_H


#include <Driver.hpp>
#include "xNode.h"
#include "xConstantNode.h"

class xVariableDefNode : xNode {
public:
	std::string name;
	xConstantNode* value;
	xNode* defaultValue;

	xNode* ex(MC::Driver* driver) override {
		xVariableDefNode* variableDef = this;

		xConstantNode* varDefValue = variableDef->value;
		if (variableDef->defaultValue != nullptr) {
			xNode* defaultValue = variableDef->defaultValue->ex(driver);
			xConstantNode* value = defaultValue->getValue();

			if (varDefValue->getType() != value->getType()) {
				std::cerr << "Warning: Defined value of type " << varDefValue->getType()
						  << " is incompatible with variable type " << value->getType()
						  << "." << std::endl;
			} else {
				varDefValue = value;
			}
		} else {
			varDefValue->defaultValue();
		}

		auto* variable = new xVariableNode;
		variable->name = variableDef->name;
		variable->value = variableDef->value;

		driver->addVariable(variable);
	}

	xConstantNode* getValue() override {
		return value;
	}

};

#endif //CRACKLE_XVARIABLEDEFNODE_H
