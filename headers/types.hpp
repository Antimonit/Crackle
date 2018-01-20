#ifndef TYPES_H_
#define TYPES_H_

#include <string>
#include <c++/iostream>

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

class ConstantNode;
class OperatorNode;
class VariableDefNode;
class VariableNode;
class FunctionDefNode;
class FunctionNode;
class ObjectDefNode;
class ObjectNode;
class ReturnNode;
class Node;


class ConstantNode {
public:
    dataTypeEnum dataType;
	union {
		int intVal;
		double doubleVal;
		bool boolVal;
		std::string stringVal;
		ObjectDefNode* objectVal;
	};
	std::string objectTypeName;
	ConstantNode() {
		dataType = typeUndefined;
		if (dataType == typeInt) {
			intVal = 0;
		} else if (dataType == typeDouble) {
			doubleVal = 0.0;
		} else if (dataType == typeString) {
			stringVal = "";
		} else if (dataType == typeBool) {
			boolVal = false;
		} else if (dataType == typeObj) {
			objectVal = nullptr;
			objectTypeName = "null";
		}
	}
	~ConstantNode() {
	}
	ConstantNode& operator=(const ConstantNode& from) {
		dataType = from.dataType;
		switch (dataType) {
			case typeInt:
				intVal = from.intVal; break;
			case typeDouble:
				doubleVal = from.doubleVal; break;
			case typeString:
				new ((void*) &stringVal) std::string(from.stringVal);
//				stringVal = from.stringVal;
				break;
			case typeBool:
				boolVal = from.boolVal; break;
			case typeObj:
				objectVal = from.objectVal;
				objectTypeName = from.objectTypeName; break;
			case typeUndefined:
				break;
		}
		return *this;
	}
};

class OperatorNode {
public:
	int oper;
	int opCount;
	Node* op[0];	/* operands, extended at runtime */

//	static void* operator new(size_t size) {
//		void *storage = malloc(size);
//		if (storage == nullptr) {
//			throw "allocation fail : no free memory";
//		}
//		return storage;
//	}
//	static void operator delete(void* ptr) {
//		free(ptr);
//	}
};

class VariableDefNode {
public:
	std::string name;
	ConstantNode value;
	Node* defaultValue;
	~VariableDefNode() {
	}
};

class VariableNode {
public:
	std::string name;
	dataTypeEnum dataType;
	VariableNode& operator=(const VariableNode& from) {
		dataType = from.dataType;
		name = from.name;
		return *this;
	}
};

class FunctionDefNode {
public:
	std::string name;
	dataTypeEnum dataType;
	Node* root;
	int paramCount;
	VariableDefNode params[0];	/* formal parameters, extended at runtime */
};

class FunctionNode {
public:
	std::string name;
	int paramCount;
	Node* params[0];	/* actual parameters, extended at runtime */
};

class ObjectDefNode {
public:
	std::string name;
	int varCount;
	VariableDefNode vars[0];
};

class ObjectNode {
public:
	std::string name;
	int varCount;
	ConstantNode vars[0];
};

class ReturnNode {
public:
	ConstantNode value;
};

class Node {
public:
    nodeTypeEnum type;
    union {
        ConstantNode constant;
        VariableDefNode variableDef;
        VariableNode variable;
		FunctionDefNode functionDef;
        FunctionNode function;
		ObjectDefNode objectDef;
		ObjectNode object;
		OperatorNode oper;
        ReturnNode ret;
    };

	Node& operator=(const Node& from) {
		if (this != &from) {
			type = from.type;
			switch (type) {
				case typeConstant: constant = from.constant; break;
				case typeVariableDef: variableDef = from.variableDef; break;
				case typeVariable: variable = from.variable; break;
				case typeFunctionDef: functionDef = from.functionDef; break;
				case typeFunction: function = from.function; break;
				case typeObjectDef: objectDef = from.objectDef; break;
				case typeObject: object = from.object; break;
				case typeOperator: oper = from.oper; break;
				case typeReturn: ret = from.ret; break;
				case typeEmpty:break;
			}
		}
		return *this;
	}

	Node();

	Node(const Node& node);

	Node(size_t size);

	~Node();

	explicit Node(nodeTypeEnum type);


	static void* operator new(size_t size) {
		void *storage = malloc(size);
		if (storage == nullptr) {
			std::cerr << "allocation fail : no free memory" << std::endl;
			throw "allocation fail : no free memory";
		}
		return storage;
	}

	static void* operator new(size_t size, nodeTypeEnum type) {
		void* storage = nullptr;
		switch (type) {
			case typeConstant:
				storage = malloc(size);
				break;
			case typeVariableDef:break;
			case typeVariable:break;
			case typeFunctionDef:break;
			case typeFunction:break;
			case typeObjectDef:break;
			case typeObject:break;
			case typeOperator:break;
			case typeReturn:break;
			case typeEmpty:break;
		}
		if (storage == nullptr) {
			std::cerr << "allocation fail : no free memory" << std::endl;
			throw "allocation fail : no free memory";
		}
		return storage;
	}

	static void* operator new(size_t size, nodeTypeEnum type, int vars) {
		void* storage = nullptr;
		switch (type) {
			case typeConstant:break;
			case typeVariableDef:break;
			case typeVariable:break;
			case typeFunctionDef:break;
			case typeFunction:break;
			case typeObjectDef:
//				(ObjectDefNode*) malloc(object->varCount * sizeof(ConstantNode*));
				storage = malloc(size + vars * sizeof(ConstantNode*));
				break;
			case typeObject:break;
			case typeOperator:
				storage = malloc(size  + vars * sizeof(type));
				break;
			case typeReturn:break;
			case typeEmpty:break;
		}

		if (storage == nullptr) {
			std::cerr << "allocation fail : no free memory" << std::endl;
			throw "allocation fail : no free memory";
		}
		return storage;
	}


	static void operator delete(void* ptr) {
		free(ptr);
	}

};


std::string dataTypeToString(dataTypeEnum type);

std::string constantValueToString(ConstantNode& constant);


#endif