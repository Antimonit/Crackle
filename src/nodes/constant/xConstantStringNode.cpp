//
// Created by DAVE on 4. 3. 2018.
//

#include "xConstantStringNode.h"

xConstantStringNode::xConstantStringNode() : xConstantNode() {
	defaultValue();
}

xConstantStringNode::xConstantStringNode(const std::string& value) : xConstantNode() {
	this->stringVal = value;
}

xConstantStringNode* xConstantStringNode::toString() const {
	return new xConstantStringNode(stringVal);
}

xConstantNode* xConstantStringNode::clone() const {
	return new xConstantStringNode(stringVal);
}
