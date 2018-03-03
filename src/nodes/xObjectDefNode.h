//
// Created by DAVE on 7. 2. 2018.
//

#ifndef CRACKLE_XOBJECTDEFNODE_H
#define CRACKLE_XOBJECTDEFNODE_H



#include "Driver.hpp"
#include "node_types.h"
#include "xNode.h"
#include "xEmptyNode.h"

class xObjectDefNode : xNode {
public:
	std::string name;
	std::vector<xVariableDefNode*> vars;

	xNode* ex(MC::Driver* driver) override {
		driver->addObject(this);
		return new xEmptyNode;
	}

	std::string print() const override {
		return "Object definition";
	}

	std::ostream& print(std::ostream& out) const override {
		return out << name << " "
	  			   << vars.size() << " vars";
	}

};


#endif //CRACKLE_XOBJECTDEFNODE_H
