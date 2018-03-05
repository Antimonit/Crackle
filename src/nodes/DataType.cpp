//
// Created by DAVE on 14. 2. 2018.
//

#include <c++/ostream>
#include "DataType.h"

std::string typeToString(DataType value) {
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
	return out << typeToString(value);
}

std::string operator+(const std::string& out, const DataType value) {
	return out + typeToString(value);
}
