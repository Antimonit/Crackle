//
// Created by DAVE on 4. 3. 2018.
//

#include "xConstantVoidNode.h"

xConstantVoidNode::xConstantVoidNode() : xConstantNode() {
	// nothing
}

xConstantVoidNode* xConstantVoidNode::toVoid() {
	return new xConstantVoidNode();
}
