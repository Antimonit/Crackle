//
// Created by DAVE on 7. 2. 2018.
//

#ifndef CRACKLE_XVARIABLENODE_H
#define CRACKLE_XVARIABLENODE_H

#include "Driver.hpp"
#include "xNode.h"
#include "xEmptyNode.h"

class xVariableNode: xNode {
public:
	std::string name;
	xConstantNode* value;

	xNode* ex(MC::Driver* driver) override {
		xVariableNode* variable = driver->findVariable(name);

		if (variable == nullptr) {
			std::cerr << "Warning: Undefined variable '" << name << "'." << std::endl;
			xEmptyNode* empty = new xEmptyNode;
			return empty;
		}

		return variable;
	}

	std::string print() const override {
		return "Variable node";
	}

	std::ostream& print(std::ostream& out) const override {
		return out << name << " "
				   << value << " vars";
	}

};


#endif //CRACKLE_XVARIABLENODE_H
