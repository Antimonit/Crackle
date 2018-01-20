//
// Created by DAVE on 29. 12. 2017.
//

#include <cstdio>
#include <c++/sstream>
#include "headers/types.hpp"

std::ostream& operator<<(std::ostream& out, const NodeType value) {
	switch (value) {
		case typeConstant:		return out << "constant";
		case typeVariableDef:	return out << "variableDef";
		case typeVariable:		return out << "variable";
		case typeFunctionDef:	return out << "functionDef";
		case typeFunction:		return out << "function";
		case typeObjectDef:		return out << "objectDef";
		case typeObject:		return out << "object";
		case typeOperator:		return out << "operator";
		case typeReturn:		return out << "return";
		case typeEmpty:			return out << "empty";
		default:				return out << "unknown";
	}
}

std::ostream& operator<<(std::ostream& out, const DataType value) {
	switch (value) {
		case typeInt:		return out << "int";
		case typeDouble:	return out << "double";
		case typeString:	return out << "string";
		case typeBool:		return out << "bool";
		case typeObj:		return out << "object";
		case typeUndefined:	return out << "undefined";
		default:			return out << "unknown";
	}
}

std::ostream& operator<<(std::ostream& out, ConstantNode& constant) {
	DataType type = constant.getType();
	switch (type) {
		case typeInt:		return out << constant.intVal;
		case typeDouble:	return out << constant.doubleVal;
		case typeString:	return out << constant.stringVal;
		case typeBool:		return out << (constant.boolVal ? "true" : "false");
		case typeObj:		return out << (constant.objectVal == nullptr ? "null" : "<object value>");
		case typeUndefined:	return out << "<undefined>";
		default:			return out << "<unknown>";
	}
}
