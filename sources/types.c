//
// Created by DAVE on 29. 12. 2017.
//

#include <stdio.h>
#include "headers/types.h"

char temp[100];

const char* dataTypeToString(dataTypeEnum type) {
	switch (type) {
		case typeInt:
			return "int";
		case typeDouble:
			return "double";
		case typeString:
			return "string";
		case typeBool:
			return "bool";
		case typeUndefined:
			return "undefined";
		default:
			return "unknown";
	}
}

const char* constantValueToString(constantNode constant) {
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
			sprintf(temp, "%s", constant.boolVal == true ? "true" : "false");
			break;
		case typeUndefined:
			sprintf(temp, "<undefined>");
			break;
	}

	return temp;
}
