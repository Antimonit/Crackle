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


extern "C" void copyConstantToVariableDef(variableDefNode* to, constantNode* from) {
	to->dataType = from->dataType;
	if (to->dataType == typeInt) {
		to->intVal = from->intVal;
	} else if (to->dataType == typeDouble) {
		to->doubleVal = from->doubleVal;
	} else if (to->dataType == typeString) {
		to->stringVal = from->stringVal;
	} else if (to->dataType == typeBool) {
		to->boolVal = from->boolVal;
	}
}

extern "C" void copyVariableDefToConstant(constantNode* to, variableDefNode* from) {
	to->dataType = from->dataType;
	if (to->dataType == typeInt) {
		to->intVal = from->intVal;
	} else if (to->dataType == typeDouble) {
		to->doubleVal = from->doubleVal;
	} else if (to->dataType == typeString) {
		to->stringVal = from->stringVal;
	} else if (to->dataType == typeBool) {
		to->boolVal = from->boolVal;
	}
}

extern "C" void defaultVariableDef(variableDefNode* def) {
	if (def->dataType == typeInt) {
		def->intVal = 0;
	} else if (def->dataType == typeDouble) {
		def->doubleVal = 0.0;
	} else if (def->dataType == typeString) {
		def->stringVal = "";
	} else if (def->dataType == typeBool) {
		def->boolVal = false;
	}
}
