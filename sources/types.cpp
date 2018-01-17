//
// Created by DAVE on 29. 12. 2017.
//

#include <cstdio>
#include <headers/types.hpp>
#include "headers/types.hpp"

char temp[100];

extern "C" const char* dataTypeToString(dataTypeEnum type) {
	switch (type) {
		case typeInt:
			return "int";
		case typeDouble:
			return "double";
		case typeString:
			return "string";
		case typeBool:
			return "bool";
		case typeObject:
			return "object";
		case typeUndefined:
			return "undefined";
		default:
			return "unknown";
	}
}

extern "C" const char* constantValueToString(constantNode constant) {
	dataTypeEnum type = constant.dataType;

	switch (type) {
		case typeInt:
			sprintf(temp, "%d", constant.intVal);
			break;
		case typeDouble:
			sprintf(temp, "%f", constant.doubleVal);
			break;
		case typeString:
			sprintf(temp, "%s", constant.stringVal);
			break;
		case typeBool:
			sprintf(temp, "%s", constant.boolVal ? "true" : "false");
			break;
		case typeObject:
			sprintf(temp, "%s", constant.objectVal == nullptr ? "null" : "object");
			break;
		case typeUndefined:
			sprintf(temp, "<undefined>");
			break;
	}

	return temp;
}
