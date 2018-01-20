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

struct constantNodeTag;		typedef struct constantNodeTag constantNode;
struct operatorNodeTag; 	typedef struct operatorNodeTag operatorNode;
struct variableDefNodeTag;	typedef struct variableDefNodeTag variableDefNode;
struct variableNodeTag;		typedef struct variableNodeTag variableNode;
struct functionDefNodeTag;	typedef struct functionDefNodeTag functionDefNode;
struct functionNodeTag;		typedef struct functionNodeTag functionNode;
struct objectDefNodeTag;	typedef struct objectDefNodeTag objectDefNode;
struct objectNodeTag;		typedef struct objectNodeTag objectNode;
struct nodeTag;				typedef struct nodeTag node;
struct returnNodeTag;		typedef struct returnNodeTag returnNode;

struct constantNodeTag {
	dataTypeEnum dataType;
	union {
		int intVal;
		double doubleVal;
		const char* stringVal;
		bool boolVal;
		objectDefNode* objectVal;
	};
	const char* objectTypeName;
};

struct operatorNodeTag {
	int oper;
	int opCount;
	node* op[0];	/* operands, extended at runtime */
};

struct variableDefNodeTag {
	const char* name;
	constantNode value;
	node* defaultValue;
};

struct variableNodeTag {
	const char* name;
	dataTypeEnum dataType;
};

struct functionDefNodeTag {
	const char* name;
	dataTypeEnum dataType;
	node* root;
	int paramCount;
	variableDefNode params[0];	/* formal parameters, extended at runtime */
};

struct functionNodeTag {
	const char* name;
	int paramCount;
	node* params[0];	/* actual parameters, extended at runtime */
};

struct objectDefNodeTag {
	const char* name;
	int varCount;
	variableDefNode vars[0];
};

struct objectNodeTag {
	const char* name;
	int varCount;
	constantNode vars[0];
};

struct returnNodeTag {
	constantNode value;
};

struct nodeTag {
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
};


const char* dataTypeToString(dataTypeEnum type);

const char* constantValueToString(constantNode constant);


#ifdef __cplusplus
}
#endif

#endif