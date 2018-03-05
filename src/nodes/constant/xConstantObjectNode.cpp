//
// Created by DAVE on 4. 3. 2018.
//

#include "xConstantObjectNode.h"

xConstantObjectNode::xConstantObjectNode() : xConstantNode() {
	defaultValue();
}

xConstantObjectNode::xConstantObjectNode(xObjectNode* value, const std::string& typeName) : xConstantNode() {
	this->objectVal = value;
	this->objectTypeName = typeName;
}

xConstantObjectNode* xConstantObjectNode::toObject() const {
	return new xConstantObjectNode(objectVal, objectTypeName);
}

xConstantNode* xConstantObjectNode::clone() const {
	return new xConstantObjectNode(objectVal, objectTypeName);
}
