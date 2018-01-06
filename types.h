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
	typeBool,
	typeUndefined
} dataTypeEnum;


typedef struct constantNodeTag {
    dataTypeEnum dataType;
    union {
        int intVal;
        double doubleVal;
		char* stringVal;
		bool boolVal;
    };
} constantNode;

typedef struct variableNodeTag {
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
	variableNode* params[0];	/* params, extended at runtime */
} functionNode;

typedef struct {
	const char* name;
	int paramCount;
	constantNode* params[0];	/* params, extended at runtime */
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