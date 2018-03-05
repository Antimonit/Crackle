//
// Created by DAVE on 7. 2. 2018.
//

#include "xReturnNode.h"
#include "xConstantNode.h"
#include "constant/xConstantVoidNode.h"

xNode* xReturnNode::ex(MC::Driver* driver) {
	return this;
}

xReturnNode::xReturnNode(xConstantNode* value) {
	this->value = value;
}

xReturnNode::xReturnNode() {
	this->value = new xConstantVoidNode();
}

std::string xReturnNode::print() const {
	return "Return";
}


std::ostream& xReturnNode::print(std::ostream& out) const {
	return out << *value;
}
