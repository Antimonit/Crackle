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

xConstantIntNode* xConstantIntNode::toInt() {
	return new xConstantIntNode(intVal);
}

xConstantDoubleNode* xConstantIntNode::toDouble() {
	return new xConstantDoubleNode((double) intVal);
}
