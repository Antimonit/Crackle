//
// Created by DAVE on 4. 3. 2018.
//

#include "xConstantIntNode.h"
#include "xConstantDoubleNode.h"

xConstantIntNode::xConstantIntNode() : xConstantNode() {
	defaultValue();
}

xConstantIntNode::xConstantIntNode(int value) : xConstantNode() {
	this->intVal = value;
}

xConstantIntNode* xConstantIntNode::toInt() const {
	return new xConstantIntNode(intVal);
}

xConstantDoubleNode* xConstantIntNode::toDouble() const {
	return new xConstantDoubleNode((double) intVal);
}

xConstantNode* xConstantIntNode::clone() const {
	return new xConstantIntNode(intVal);
}
