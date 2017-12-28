#ifndef TYPES_H_
#define TYPES_H_

typedef enum { 
    typeConstant,
    typeVariable,
    typeOperator,
    typeFunctionDef,
    typeFunctionCall,
    typeReturn,
    typeEmpty
} nodeEnum;

typedef enum {
    typeInt,
    typeDouble,
	typeString
} typeEnum;



/* CONSTANT */
typedef struct {
    typeEnum type;
    union {
        int intVal;
        double doubleVal;
		char* stringVal;
    };
} constantNode;

/* VARIABLE */
typedef struct {
    const char* name;
} variableNode;

/* OPERATOR */
typedef struct {
    int oper;
    int opCount;
    struct nodeTag* op[1];	/* operands, extended at runtime */
} operatorNode;

/* FUNCTION DEFINITION */
typedef struct {
	const char* name;
	variableNode* params[1];
	struct nodeTag* root;
} functionNode;

/* FUNCTION CALL */
typedef struct {
	const char* name;
	constantNode* params[1];
} functionCallNode;

/* RETURN */
typedef struct {
	constantNode value;
} returnNode;

/* NODE */
typedef struct nodeTag {
    nodeEnum type;
    union {
        constantNode constant;
        variableNode variable;
        operatorNode oper;
        functionNode function;
        functionCallNode functionCall;
        returnNode ret;
    };
} node;

#endif