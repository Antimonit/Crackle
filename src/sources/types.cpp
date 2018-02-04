//
// Created by DAVE on 29. 12. 2017.
//

#include <cstdio>
#include <c++/sstream>
#include "headers/types.hpp"

std::ostream& operator<<(std::ostream& out, const Node::Type value) {
	switch (value) {
		case Node::Constant:	return out << "constant";
		case Node::VariableDef:	return out << "variableDef";
		case Node::Variable:	return out << "variable";
		case Node::FunctionDef:	return out << "functionDef";
		case Node::Function:	return out << "function";
		case Node::ObjectDef:	return out << "objectDef";
		case Node::Object:		return out << "object";
		case Node::Operator:	return out << "operator";
		case Node::Return:		return out << "return";
		case Node::Empty:		return out << "empty";
		default:				return out << "<unknown>";
	}
}
std::string toString(DataType value) {
	switch (value) {
		case typeInt:		return "int";
		case typeDouble:	return "double";
		case typeString:	return "string";
		case typeBool:		return "bool";
		case typeVoid:		return "void";
		case typeObject:	return "object";
		case typeUndefined:	return "<undefined>";
		default:			return "<unknown>";
	}
}
std::ostream& operator<<(std::ostream& out, const DataType value) {
	return out << toString(value);
}
std::string operator+(const std::string& out, const DataType value) {
	return out + toString(value);
}

std::ostream& operator<<(std::ostream& out, ConstantNode& constant) {
	DataType type = constant.getType();
	switch (type) {
		case typeInt:		return out << constant.intVal;
		case typeDouble:	return out << constant.doubleVal;
		case typeBool:		return out << (constant.boolVal ? "true" : "false");
		case typeString:	return out << constant.stringVal;
		case typeVoid:		return out << "void";
		case typeObject:
			if (constant.objectVal == nullptr)
				return out << "null";
			else
				return out << "<object " << constant.objectTypeName << ">";
		case typeUndefined:	return out << "<undefined>";
		default:			return out << "<unknown>";
	}
}

std::ostream& operator<<(std::ostream& out, VariableDefNode& variableDef) {
	return out << variableDef.value.getType() << " " << variableDef.name;
}

std::ostream& operator<<(std::ostream& out, FunctionDefNode& functionDef) {
	return out << functionDef.dataType << " "
			   << functionDef.name << " "
			   << functionDef.params.size() << " params";
}

