//
// Created by DAVE on 7. 1. 2018.
//

#include <stdio.h>
#include <headers/types.h>
#include "headers/node_helpers.h"
#include "headers/types.h"

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

dataTypeEnum widenNodes(constantNode* a, constantNode* b) {
	dataTypeEnum max = maxType(a, b);
	widenNode(a, max);
	widenNode(b, max);
	return max;
}
