#ifndef TYPES_H_
#define TYPES_H_

typedef enum {
	false,
	true
} bool;

typedef enum {
    typeConstant,
    typeVariable,
    typeOperator,
	typeFunctionDef,
    typeFunctionCall,
    typeReturn,
    typeEmpty
} nodeTypeEnum;

typedef enum {
    typeInt,
    typeDouble,
	typeString,
	typeBool
} dataTypeEnum;


typedef struct {
    dataTypeEnum type;
    union {
        int intVal;
        double doubleVal;
		char* stringVal;
		bool boolVal;
    };
} constantNode;

typedef struct {
	dataTypeEnum dataType; // TODO add type and use it to infer types from function parameters
    const char* name;
} variableNode;

typedef struct {
    int oper;
    int opCount;
    struct nodeTag* op[1];	/* operands, extended at runtime */
} operatorNode;

typedef struct {
	const char* name;
	dataTypeEnum dataType;
	variableNode* params[1];
	struct nodeTag* root;
} functionNode;

typedef struct {
	const char* name;
	constantNode* params[1];
} functionCallNode;

typedef struct {
	constantNode value;
} returnNode;

typedef struct nodeTag {
    nodeTypeEnum type;
    union {
        constantNode constant;
        variableNode variable;
        operatorNode oper;
        functionNode function;
        functionCallNode functionCall;
        returnNode ret;
    };
} node;


const char* getDataTypeString(dataTypeEnum type);

const char* getConstantValueString(constantNode constant);


#endif