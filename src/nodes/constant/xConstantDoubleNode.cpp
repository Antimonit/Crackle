//
// Created by DAVE on 4. 3. 2018.
//

#include "xConstantDoubleNode.h"
#include "xConstantIntNode.h"

xConstantDoubleNode::xConstantDoubleNode() : xConstantNode() {
	defaultValue();
}

xConstantDoubleNode::xConstantDoubleNode(double value) : xConstantNode() {
	this->doubleVal = value;
}

xConstantIntNode* xConstantDoubleNode::toInt() const {
	return new xConstantIntNode((int) doubleVal);
}

xConstantDoubleNode* xConstantDoubleNode::toDouble() const {
	return new xConstantDoubleNode(doubleVal);
}

xConstantNode* xConstantDoubleNode::clone() const {
	return new xConstantDoubleNode(doubleVal);
}
