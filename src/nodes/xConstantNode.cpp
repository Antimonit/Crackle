//
// Created by DAVE on 7. 2. 2018.
//

#include "xConstantNode.h"

xConstantNode* xConstantNode::ex(MC::Driver* driver) {
	return this;
}

xConstantNode* xConstantNode::getValue() {
	return this;
}

//std::ostream& xConstantNode::print(std::ostream& out) const {
//	DataType type = getType();
//	switch (type) {
//		case typeUndefined:	return out << "<undefined>";
//		default:			return out << "<unknown>";
//	}
//}
