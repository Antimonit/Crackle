//
// Created by DAVE on 7. 2. 2018.
//

#include "xObjectNode.h"
#include "Driver.hpp"

#include "xVariableNode.h"
#include "xVariableDefNode.h"
#include "xObjectDefNode.h"
#include "xConstantNode.h"
#include "xEmptyNode.h"
#include "constant/xConstantObjectNode.h"

xNode* xObjectNode::ex(MC::Driver* driver) {
	xObjectDefNode* objectDef = driver->findObject(name);
	if (objectDef == nullptr) {
		std::cerr << "Warning: Undefined object '" << name
				  << "'." << std::endl;
		return new xEmptyNode;
	}


	auto* objectVal = driver->allocateNewObject();
	if (objectVal == nullptr) {
		std::cerr << "ERROR: Heap is full. Cannot allocate new objects." << std::endl;
		return new xEmptyNode;
	}

	for (auto* varDef : objectDef->vars) {
		auto* var = new xVariableNode(varDef->name, varDef->value);
		if (varDef->defaultValue != nullptr) {
			xNode* defaultVal = varDef->defaultValue->ex(driver);
			var->value = dynamic_cast<xConstantNode*>(defaultVal);
		} else {
			var->value->defaultValue();
		}
		objectVal->vars.push_back(var);
	}

	return new xConstantObjectNode(objectVal, name);
}

std::string xObjectNode::print() const {
	return "Object";
}

std::ostream& xObjectNode::print(std::ostream& out) const {
	return out << name << " "
			   << vars.size() << " vars";
}
