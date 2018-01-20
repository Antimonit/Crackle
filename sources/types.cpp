//
// Created by DAVE on 29. 12. 2017.
//

#include <cstdio>
#include <c++/sstream>
#include "headers/types.hpp"

std::ostringstream stringStream;

std::string dataTypeToString(dataTypeEnum type) {
	switch (type) {
		case typeInt:
			return "int";
		case typeDouble:
			return "double";
		case typeString:
			return "string";
		case typeBool:
			return "bool";
		case typeObj:
			return "object";
		case typeUndefined:
			return "undefined";
		default:
			return "unknown";
	}
}

std::string constantValueToString(ConstantNode& constant) {
	stringStream.clear();
	stringStream.str("");

	dataTypeEnum type = constant.dataType;
	switch (type) {
		case typeInt:
			stringStream << constant.intVal;
			break;
		case typeDouble:
			stringStream << constant.doubleVal;
			break;
		case typeString:
			stringStream << constant.stringVal;
			break;
		case typeBool:
			stringStream << (constant.boolVal ? "true" : "false");
			break;
		case typeObj:
			stringStream << (constant.objectVal == nullptr ? "null" : "<object value>");
			break;
		case typeUndefined:
			stringStream << "<undefined>";
			break;
	}

	return stringStream.str();
}


Node::Node(const Node& node) {
	type = node.type;
	switch (type) {
		case typeConstant:
			constant = node.constant;
			break;
		case typeVariableDef:
			variableDef = node.variableDef;
			break;
		case typeVariable:
			variable = node.variable;
			break;
		case typeFunctionDef:
			functionDef = node.functionDef;
			break;
		case typeFunction:
			function = node.function;
			break;
		case typeObjectDef:
			objectDef = node.objectDef;
			break;
		case typeObject:
			object = node.object;
			break;
		case typeOperator:
			oper = node.oper;
			break;
		case typeReturn:
			ret = node.ret;
			break;
		case typeEmpty:
			break;
	}
}

Node::Node(size_t size) {

}

Node::Node() {
	type = typeEmpty;
}

Node::Node(nodeTypeEnum type) {
	this->type = type;
}

Node::~Node() {
}