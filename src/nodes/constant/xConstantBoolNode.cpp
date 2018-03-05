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

xConstantBoolNode* xConstantBoolNode::toBool() {
	return new xConstantBoolNode(boolVal);
}

