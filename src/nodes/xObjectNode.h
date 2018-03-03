//
// Created by DAVE on 7. 2. 2018.
//

#ifndef CRACKLE_XOBJECTNODE_H
#define CRACKLE_XOBJECTNODE_H

#include "xNode.h"
#include "xVariableNode.h"
#include "xVariableDefNode.h"
#include "xObjectDefNode.h"

class xObjectNode : xNode {
public:
	bool marked;
	std::string name;
	std::vector<xVariableNode*> vars;

	xNode* ex(MC::Driver* driver) override {
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
			auto* var = new xVariableNode;
			var->name = varDef->name;
			var->value = varDef->value;
			if (varDef->defaultValue != nullptr) {
				xNode* defaultVal = varDef->defaultValue->ex(driver);
				xConstantNode* defaultConstantVal = dynamic_cast<xConstantNode*>(defaultVal);
				var->value = defaultConstantVal;
			} else {
				var->value->defaultValue();
			}
			objectVal->vars.push_back(var);
		}

		return new xConstantNode(objectVal, name);
	}

	std::string print() const override {
		return "Object";
	}

	std::ostream& print(std::ostream& out) const override {
		return out << name << " "
				   << vars.size() << " vars";
	}

};


#endif //CRACKLE_XOBJECTNODE_H
