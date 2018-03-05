//
// Created by DAVE on 4. 3. 2018.
//

#include "xConstantVoidNode.h"

xConstantVoidNode::xConstantVoidNode() : xConstantNode() {
	// nothing
}

xConstantVoidNode* xConstantVoidNode::toVoid() const {
	return new xConstantVoidNode();
}

xConstantNode* xConstantVoidNode::clone() const {
	return new xConstantVoidNode();
}
