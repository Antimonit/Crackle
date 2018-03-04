//
// Created by DAVE on 7. 2. 2018.
//


#include "xObjectDefNode.h"
#include "Driver.hpp"

#include "xEmptyNode.h"

xNode* xObjectDefNode::ex(MC::Driver* driver) {
	driver->addObject(this);
	return new xEmptyNode;
}

std::string xObjectDefNode::print() const {
	return "Object definition";
}

std::ostream& xObjectDefNode::print(std::ostream& out) const {
	return out << name << " "
			   << vars.size() << " vars";
}
