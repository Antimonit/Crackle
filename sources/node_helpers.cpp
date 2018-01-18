//
// Created by DAVE on 7. 1. 2018.
//

#include <stdio.h>
#include <headers/types.hpp>
#include "headers/node_helpers.hpp"
#include "headers/types.hpp"

dataTypeEnum maxType(constantNode* a, constantNode* b) {
	dataTypeEnum typeA = a->dataType;
	dataTypeEnum typeB = b->dataType;
	if (typeA == typeInt && typeB == typeInt) {
		return typeInt;
	} else if ((typeA == typeDouble || typeB == typeDouble) && (typeA == typeInt || typeB == typeInt)) {
		return typeDouble;
	} else {
		return typeUndefined;
	}
}

void widenNode(constantNode* a, dataTypeEnum maxType) {
	if (a->dataType == maxType) {
		return;
	}
	if (a->dataType == typeInt && maxType == typeDouble) {
		a->dataType = typeDouble;
		a->doubleVal = (double) a->intVal;
	} else {
		printf("Warning: Widening of type %s to type %s.\n", dataTypeToString(a->dataType), dataTypeToString(maxType));
	}
}

extern "C" dataTypeEnum widenNodes(constantNode* a, constantNode* b) {
	dataTypeEnum max = maxType(a, b);
	widenNode(a, max);
	widenNode(b, max);
	return max;
}


extern "C" void defaultConstant(constantNode* def) {
	if (def->dataType == typeInt) {
		def->intVal = 0;
	} else if (def->dataType == typeDouble) {
		def->doubleVal = 0.0;
	} else if (def->dataType == typeString) {
		def->stringVal = "";
	} else if (def->dataType == typeBool) {
		def->boolVal = false;
	} else if (def->dataType == typeObj) {
		def->objectVal = nullptr;
		def->objectTypeName = "null";
	}
}
