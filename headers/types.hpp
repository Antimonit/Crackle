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
	typeFunctionDef,
    typeFunction,
	typeObjectDef,
	typeObject,
	typeOperator,
    typeReturn,
    typeEmpty
} nodeTypeEnum;

typedef enum {
    typeInt,
    typeDouble,
	typeString,
	typeBool,
	typeObj,
	typeUndefined
} dataTypeEnum;

struct objectDefNodeTag;

typedef struct {
    dataTypeEnum dataType;
    union {
        int intVal;
        double doubleVal;
		const char* stringVal;
		bool boolVal;
		struct objectDefNodeTag* objectVal;
    };
	const char* objectTypeName;
} constantNode;

typedef struct {
	int oper;
	int opCount;
	struct nodeTag* op[0];	/* operands, extended at runtime */
} operatorNode;


typedef struct {
	const char* name;
	constantNode value;
	struct nodeTag* defaultValue;
} variableDefNode;

typedef struct {
	const char* name;
	dataTypeEnum dataType;
} variableNode;

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

typedef struct objectDefNodeTag {
	const char* name;
	int varCount;
	variableDefNode vars[0];
} objectDefNode;

typedef struct {
	const char* name;
	int varCount;
	constantNode vars[0];
} objectNode;


typedef struct {
	constantNode value;
} returnNode;

typedef struct nodeTag {
    nodeTypeEnum type;
    union {
        constantNode constant;
        variableDefNode variableDef;
        variableNode variable;
		functionDefNode functionDef;
        functionNode function;
		objectDefNode objectDef;
		objectNode object;
		operatorNode oper;
        returnNode ret;
    };
} node;


const char* dataTypeToString(dataTypeEnum type);

const char* constantValueToString(constantNode constant);


#ifdef __cplusplus
}
#endif

#endif