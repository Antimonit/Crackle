//
// Created by DAVE on 4. 3. 2018.
//

#include "xConstantBoolNode.h"

xConstantBoolNode::xConstantBoolNode() : xConstantNode() {
	defaultValue();
}

xConstantBoolNode::xConstantBoolNode(bool value) : xConstantNode() {
	this->boolVal = value;
}

xConstantBoolNode* xConstantBoolNode::toBool() const {
	return new xConstantBoolNode(boolVal);
}

xConstantNode* xConstantBoolNode::clone() const {
	return new xConstantBoolNode(boolVal);
}

