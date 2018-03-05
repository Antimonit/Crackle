//
// Created by DAVE on 4. 3. 2018.
//

#include "xConstantDoubleNode.h"
#include "xConstantIntNode.h"

xConstantDoubleNode::xConstantDoubleNode(double value) : xConstantNode() {
	this->doubleVal = value;
}

xConstantIntNode* xConstantDoubleNode::toInt() {
	return new xConstantIntNode((int) doubleVal);
}

xConstantDoubleNode* xConstantDoubleNode::toDouble() {
	return new xConstantDoubleNode(doubleVal);
}
