#ifndef TYPES_H_
#define TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

typedef enum {
    typeConstant,
	typeVariableDef,
    typeVariable,
    typeOperator,
	typeFunctionDef,
    typeFunction,
    typeReturn,
    typeEmpty
} nodeTypeEnum;

typedef enum {
    typeInt,
    typeDouble,
	typeString,
	typeBool,
	typeUndefined
} dataTypeEnum;


typedef struct {
    dataTypeEnum dataType;
    union {
        int intVal;
        double doubleVal;
		const char* stringVal;
		bool boolVal;
    };
} constantNode;

typedef struct {
	dataTypeEnum dataType;
	const char* name;
	union {
		int intVal;
		double doubleVal;
		const char* stringVal;
		bool boolVal;
	};
} variableDefNode;

typedef struct {
	dataTypeEnum dataType;
    const char* name;
} variableNode;

typedef struct {
    int oper;
    int opCount;
    struct nodeTag* op[0];	/* operands, extended at runtime */
} operatorNode;

typedef struct {
	const char* name;
	dataTypeEnum dataType;
	struct nodeTag* root;
	int paramCount;
	variableDefNode params[0];	/* formal parameters, extended at runtime */
} functionDefNode;

typedef struct {
	const char* name;
	int paramCount;
	struct nodeTag* params[0];	/* actual parameters, extended at runtime */
} functionNode;

typedef struct {
	constantNode value;
} returnNode;

typedef struct nodeTag {
    nodeTypeEnum type;
    union {
        constantNode constant;
        variableDefNode variableDef;
        variableNode variable;
        operatorNode oper;
		functionDefNode functionDef;
        functionNode function;
        returnNode ret;
    };
} node;


const char* dataTypeToString(dataTypeEnum type);

const char* constantValueToString(constantNode constant);


#ifdef __cplusplus
}
#endif

#endif