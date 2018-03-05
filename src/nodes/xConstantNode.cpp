//
// Created by DAVE on 7. 2. 2018.
//

#include "xConstantNode.h"
#include "constant/xConstantIntNode.h"
#include "constant/xConstantDoubleNode.h"
#include "constant/xConstantBoolNode.h"
#include "constant/xConstantStringNode.h"
#include "constant/xConstantObjectNode.h"
#include "constant/xConstantVoidNode.h"

xConstantNode* xConstantNode::ex(MC::Driver* driver) {
	return this;
}

xConstantNode* xConstantNode::getValue() {
	return this;
}

xConstantNode* xConstantNode::fromType(DataType type) {
	switch (type) {
		case typeInt:		return new xConstantIntNode();
		case typeDouble:	return new xConstantDoubleNode();
		case typeBool:		return new xConstantBoolNode();
		case typeString:	return new xConstantStringNode();
		case typeObject:	return new xConstantObjectNode();
		case typeVoid:		return new xConstantVoidNode();
		default: 			throw "Invalid type.";
	}
}

//std::ostream& xConstantNode::print(std::ostream& out) const {
//	DataType type = getType();
//	switch (type) {
//		case typeUndefined:	return out << "<undefined>";
//		default:			return out << "<unknown>";
//	}
//}
