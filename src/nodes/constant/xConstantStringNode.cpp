//
// Created by DAVE on 4. 3. 2018.
//

#include "xConstantStringNode.h"

xConstantStringNode::xConstantStringNode(const std::string& value) : xConstantNode() {
	this->stringVal = value;
}

xConstantStringNode* xConstantStringNode::toString() {
	return new xConstantStringNode(stringVal);
}
