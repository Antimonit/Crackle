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
} dataTypeNode;

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
    const char* name;
} variableNode;

typedef struct {
    int oper;
    int opCount;
    struct nodeTag* op[1];	/* operands, extended at runtime */
} operatorNode;

/* FUNCTION DEFINITION */
typedef struct {
	const char* name;
	dataTypeEnum dataType;
	variableNode* params[1];
	struct nodeTag* root;
} functionNode;

/* FUNCTION CALL */
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
        dataTypeNode dataType;
    };
} node;

#endif