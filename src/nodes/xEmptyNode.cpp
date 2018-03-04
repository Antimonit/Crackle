//
// Created by DAVE on 7. 2. 2018.
//

#include "xEmptyNode.h"
#include "Driver.hpp"

xNode* xEmptyNode::ex(MC::Driver* driver) {
	return this;
}

std::string xEmptyNode::print() const {
	return "EMPTY";
}

std::ostream& xEmptyNode::print(std::ostream& out) const {
	return out;
}
