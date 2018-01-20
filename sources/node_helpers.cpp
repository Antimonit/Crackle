//
// Created by DAVE on 7. 1. 2018.
//

#include <iostream>
#include "headers/types.hpp"
#include "headers/node_helpers.hpp"

dataTypeEnum maxType(ConstantNode* a, ConstantNode* b) {
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

void widenNode(ConstantNode* a, dataTypeEnum maxType) {
	if (a->dataType == maxType) {
		return;
	}
	if (a->dataType == typeInt && maxType == typeDouble) {
		a->dataType = typeDouble;
		a->doubleVal = (double) a->intVal;
	} else {
		std::cerr << "Warning: Widening of type " << dataTypeToString(a->dataType)
				  << " to type " << dataTypeToString(maxType)
				  << "." << std::endl;
	}
}

dataTypeEnum widenNodes(ConstantNode* a, ConstantNode* b) {
	dataTypeEnum max = maxType(a, b);
	widenNode(a, max);
	widenNode(b, max);
	return max;
}


void defaultConstant(ConstantNode* def) {
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
