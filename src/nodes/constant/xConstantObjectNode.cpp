//
// Created by DAVE on 4. 3. 2018.
//

#include "xConstantObjectNode.h"

xConstantObjectNode::xConstantObjectNode(xObjectNode* value, const std::string& typeName) : xConstantNode() {
	this->objectVal = value;
	this->objectTypeName = typeName;
}

xConstantObjectNode* xConstantObjectNode::toObject() {
	return new xConstantObjectNode(objectVal, objectTypeName);
}
