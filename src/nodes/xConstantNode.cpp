//
// Created by DAVE on 7. 2. 2018.
//

#include <c++/iostream>
#include "xConstantNode.h"
#include "xObjectNode.h"

xNode* xConstantNode::ex(MC::Driver* driver) {
	auto* result = new xConstantNode(typeUndefined);
	result->copyConstantNode(*this);
	return result;
}

xConstantNode* xConstantNode::getValue() {
	return this;
}

std::string xConstantNode::print() const {
	return "Constant";
}

std::ostream& xConstantNode::print(std::ostream& out) const {
	DataType type = getType();
	switch (type) {
		case typeInt:		return out << intVal;
		case typeDouble:	return out << doubleVal;
		case typeBool:		return out << (boolVal ? "true" : "false");
		case typeString:	return out << stringVal;
		case typeVoid:		return out << "void";
		case typeObject:
			if (objectVal == nullptr)
				return out << "null";
			else
				return out << "<object " << objectTypeName << ">";
		case typeUndefined:	return out << "<undefined>";
		default:			return out << "<unknown>";
	}
}
