#ifndef TYPES_H_
#define TYPES_H_

#include <c++/string>
#include <c++/iostream>
#include <c++/vector>

enum NodeType {
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
};

std::ostream& operator<<(std::ostream& out, NodeType value);

enum DataType {
    typeInt,
    typeDouble,
	typeString,
	typeBool,
	typeObj,
	typeUndefined
};

std::ostream& operator<<(std::ostream& out, DataType value);


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
private:
	DataType dataType {typeUndefined};
public:
	union {
		int intVal;
		double doubleVal;
		bool boolVal;
		std::string stringVal;
		ObjectDefNode* objectVal;
	};
	std::string objectTypeName;

	DataType getType() {
		return dataType;
	}

	void setType(DataType type) {
		this->dataType = type;
		switch (type) {
			case typeString: 	new ((void*) &stringVal) std::string; break;
			default:			break;
		}
	}

	ConstantNode& operator=(const ConstantNode& other) {
		if (this != &other) {
			setType(other.dataType);
			switch (dataType) {
				case typeInt:		intVal = other.intVal; break;
				case typeDouble:	doubleVal = other.doubleVal; break;
				case typeBool:		boolVal = other.boolVal; break;
				case typeString:	stringVal = other.stringVal; break;
				case typeObj:
					objectVal = other.objectVal;
					objectTypeName = other.objectTypeName;
					break;
				case typeUndefined:
					break;
			}
		}
		return *this;
	}

	ConstantNode() {
		setType(typeUndefined);
	}

	ConstantNode(const ConstantNode& other) {
		setType(other.dataType);
		switch (dataType) {
			case typeInt:		intVal = other.intVal; break;
			case typeDouble:	doubleVal = other.doubleVal; break;
			case typeBool:		boolVal = other.boolVal; break;
			case typeString:	stringVal = other.stringVal; break;
			case typeObj:		objectVal = other.objectVal; break;
			case typeUndefined:	break;
		}
	}

	ConstantNode(DataType type) {
		setType(type);
	}

	~ConstantNode() {
	}

};

std::ostream& operator<<(std::ostream& out, ConstantNode& constant);

class OperatorNode {
public:
	int oper;
	std::vector<Node*> op;
};

class VariableDefNode {
public:
	std::string name;
	ConstantNode value;
	Node* defaultValue;
};

class VariableNode {
public:
	std::string name;
	DataType dataType;
};

class FunctionDefNode {
public:
	std::string name;
	DataType dataType;
	Node* root;
	std::vector<VariableDefNode> params;
};

class FunctionNode {
public:
	std::string name;
	std::vector<Node*> params;
};

class ObjectDefNode {
public:
	std::string name;
	std::vector<VariableDefNode> vars;
};

class ObjectNode {
public:
	std::string name;
	std::vector<ConstantNode> vars;
};

class ReturnNode {
public:
	ConstantNode value;
};

class Node {
private:
	NodeType type{typeEmpty};
public:
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

	NodeType getType() {
		return type;
	}

	void setType(NodeType type) {
		this->type = type;
		switch (type) {
			case typeConstant: 		new ((void*) &constant) ConstantNode; break;
			case typeVariableDef: 	new ((void*) &variableDef) VariableDefNode; break;
			case typeVariable: 		new ((void*) &variable) VariableNode; break;
			case typeFunctionDef: 	new ((void*) &functionDef) FunctionDefNode; break;
			case typeFunction: 		new ((void*) &function) FunctionNode; break;
			case typeObjectDef: 	new ((void*) &objectDef) ObjectDefNode; break;
			case typeObject: 		new ((void*) &object) ObjectNode; break;
			case typeOperator: 		new ((void*) &oper) OperatorNode; break;
			case typeReturn: 		new ((void*) &ret) ReturnNode; break;
			case typeEmpty:			break;
		}
	}

	Node& operator=(const Node& other) {
		if (this != &other) {
			setType(other.type);
			switch (type) {
				case typeConstant:		constant = other.constant; break;
				case typeVariableDef:	variableDef = other.variableDef; break;
				case typeVariable:		variable = other.variable; break;
				case typeFunctionDef:	functionDef = other.functionDef; break;
				case typeFunction:		function = other.function; break;
				case typeObjectDef:		objectDef = other.objectDef; break;
				case typeObject: 		object = other.object; break;
				case typeOperator: 		oper = other.oper; break;
				case typeReturn: 		ret = other.ret; break;
				case typeEmpty:			break;
			}
		}
		return *this;
	}

	Node() {
		setType(typeEmpty);
	}

	Node(const Node& other) {
		setType(other.type);
		switch (type) {
			case typeConstant:		constant = other.constant; break;
			case typeVariableDef:	variableDef = other.variableDef; break;
			case typeVariable:		variable = other.variable; break;
			case typeFunctionDef:	functionDef = other.functionDef; break;
			case typeFunction:		function = other.function; break;
			case typeObjectDef:		objectDef = other.objectDef; break;
			case typeObject: 		object = other.object; break;
			case typeOperator: 		oper = other.oper; break;
			case typeReturn: 		ret = other.ret; break;
			case typeEmpty:			break;
		}
	}

	Node(NodeType type) {
		setType(type);
	}

	~Node() {
	}


	static void* operator new(size_t size) {
		void *storage = malloc(size);
		if (storage == nullptr) {
			std::cerr << "allocation fail : no free memory" << std::endl;
			throw "allocation fail : no free memory";
		}
		return storage;
	}

	static void* operator new(size_t size, NodeType type) {
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

	static void* operator new(size_t size, NodeType type, int varCount) {
		void* storage = nullptr;
		switch (type) {
			case typeConstant:break;
			case typeVariableDef:break;
			case typeVariable:break;
			case typeFunctionDef:
				storage = malloc(size + varCount * sizeof(VariableDefNode));
				break;
			case typeFunction:
				storage = malloc(size + varCount * sizeof(ConstantNode*));
				break;
			case typeObjectDef:
				storage = malloc(size + varCount * sizeof(VariableDefNode));
				break;
			case typeObject:break;
			case typeOperator:
				storage = malloc(size  + varCount * sizeof(Node*));
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


#endif