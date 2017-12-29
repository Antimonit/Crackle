//
// Created by DAVE on 29. 12. 2017.
//

#include <stdio.h>
#include "types.h"

char temp[100];

const char* getDataTypeString(dataTypeEnum type) {
    switch (type) {
        case typeInt:
            return "int";
        case typeDouble:
            return "double";
        case typeString:
            return "string";
        case typeBool:
            return "bool";
        default:
            return "unknown";
    }
}

const char* getConstantValueString(constantNode constant) {
    dataTypeEnum type = constant.type;

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
    }

    return temp;
}
