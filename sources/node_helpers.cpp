//
// Created by DAVE on 7. 1. 2018.
//

#include <iostream>
#include "headers/types.hpp"
#include "headers/node_helpers.hpp"

DataType maxType(ConstantNode& a, ConstantNode& b) {
	DataType typeA = a.getType();
	DataType typeB = b.getType();
	if (typeA == typeInt && typeB == typeInt) {
		return typeInt;
	} else if ((typeA == typeDouble || typeB == typeDouble) && (typeA == typeInt || typeB == typeInt)) {
		return typeDouble;
	} else if (typeA == typeString && typeB == typeString) {
		return typeString;
	} else {
		return typeUndefined;
	}
}

void widenNode(ConstantNode& a, DataType maxType) {
	if (a.getType() == maxType) {
		return;
	}
	if (a.getType() == typeInt && maxType == typeDouble) {
		a.setType(typeDouble);
		a.doubleVal = (double) a.intVal;
	} else {
		std::cerr << "Warning: Widening of type " << a.getType()
				  << " to type " << maxType
				  << "." << std::endl;
	}
}

DataType widenNodes(ConstantNode& a, ConstantNode& b) {
	DataType max = maxType(a, b);
	widenNode(a, max);
	widenNode(b, max);
	return max;
}

void defaultConstant(ConstantNode& def) {
	if (def.getType() == typeInt) {
		def.intVal = 0;
	} else if (def.getType() == typeDouble) {
		def.doubleVal = 0.0;
	} else if (def.getType() == typeString) {
		def.stringVal = "";
	} else if (def.getType() == typeBool) {
		def.boolVal = false;
	} else if (def.getType() == typeObj) {
		def.objectVal = nullptr;
		def.objectTypeName = "null";
	}
}
