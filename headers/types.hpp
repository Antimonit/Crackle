#ifndef TYPES_H_
#define TYPES_H_

#include <c++/string>
#include <c++/iostream>
#include <c++/vector>


enum DataType {
    typeInt,
    typeDouble,
	typeString,
	typeBool,
	typeObject,
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

	void copyConstantNode(const ConstantNode& other) {
		setType(other.dataType);
		switch (dataType) {
			case typeInt:		intVal = other.intVal; break;
			case typeDouble:	doubleVal = other.doubleVal; break;
			case typeBool:		boolVal = other.boolVal; break;
			case typeString:	stringVal = other.stringVal; break;
			case typeObject:
				objectVal = other.objectVal;
				objectTypeName = other.objectTypeName;
				break;
			case typeUndefined:
				break;
		}
	}

public:
	union {
		int intVal;
		double doubleVal;
		bool boolVal;
		std::string stringVal;
		ObjectNode* objectVal;
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
			copyConstantNode(other);
		}
		return *this;
	}

	ConstantNode() {
		setType(typeUndefined);
	}

	ConstantNode(const ConstantNode& other) {
		copyConstantNode(other);
	}

	ConstantNode(DataType type) {
		setType(type);
	}

	~ConstantNode() {
	}

};

std::ostream& operator<<(std::ostream& out, ConstantNode& constant);

struct OperatorNode {
	int oper;
	std::vector<Node*> op;
};

struct VariableDefNode {
	std::string name;
	ConstantNode value;
	Node* defaultValue;
};
std::ostream& operator<<(std::ostream& out, VariableDefNode& variableDef);

struct VariableNode {
	std::string name;
	ConstantNode value;
};

struct FunctionDefNode {
	std::string name;
	DataType dataType;
	Node* root;
	std::vector<VariableDefNode> params;	// formal parameters
};
std::ostream& operator<<(std::ostream& out, FunctionDefNode& variableDef);

struct FunctionNode {
	std::string name;
	std::vector<Node*> params;	// actual parameters
};

struct ObjectDefNode {
	std::string name;
	std::vector<VariableDefNode> vars;
};

struct ObjectNode {
	std::string name;
	std::vector<VariableNode*> vars;
};

struct ReturnNode {
	ConstantNode value;
};

class Node {
public:
	enum Type {
		Constant,
		VariableDef,
		Variable,
		FunctionDef,
		Function,
		ObjectDef,
		Object,
		Operator,
		Return,
		Empty
	};

private:
	Type type{Empty};

public:

	union {
        ConstantNode constant;
        VariableDefNode variableDef;
        VariableNode* variable;
		FunctionDefNode functionDef;
        FunctionNode function;
		ObjectDefNode objectDef;
		ObjectNode object;
		OperatorNode oper;
        ReturnNode ret;
    };

	Type getType() {
		return type;
	}

	void setType(Type type) {
		this->type = type;
		switch (type) {
			case Constant: 		new ((void*) &constant) ConstantNode; break;
			case VariableDef: 	new ((void*) &variableDef) VariableDefNode; break;
			case Variable: 		new ((void*) &variable) VariableNode; break;
			case FunctionDef: 	new ((void*) &functionDef) FunctionDefNode; break;
			case Function: 		new ((void*) &function) FunctionNode; break;
			case ObjectDef: 	new ((void*) &objectDef) ObjectDefNode; break;
			case Object: 		new ((void*) &object) ObjectNode; break;
			case Operator: 		new ((void*) &oper) OperatorNode; break;
			case Return: 		new ((void*) &ret) ReturnNode; break;
			case Empty:			break;
		}
	}

	Node& operator=(const Node& other) {
		if (this != &other) {
			setType(other.type);
			switch (type) {
				case Constant:		constant = other.constant; break;
				case VariableDef:	variableDef = other.variableDef; break;
				case Variable:		variable = other.variable; break;
				case FunctionDef:	functionDef = other.functionDef; break;
				case Function:		function = other.function; break;
				case ObjectDef:		objectDef = other.objectDef; break;
				case Object: 		object = other.object; break;
				case Operator: 		oper = other.oper; break;
				case Return: 		ret = other.ret; break;
				case Empty:			break;
			}
		}
		return *this;
	}

	Node() {
		setType(Empty);
	}

	Node(const Node& other) {
		setType(other.type);
		switch (type) {
			case Constant:		constant = other.constant; break;
			case VariableDef:	variableDef = other.variableDef; break;
			case Variable:		variable = other.variable; break;
			case FunctionDef:	functionDef = other.functionDef; break;
			case Function:		function = other.function; break;
			case ObjectDef:		objectDef = other.objectDef; break;
			case Object: 		object = other.object; break;
			case Operator: 		oper = other.oper; break;
			case Return: 		ret = other.ret; break;
			case Empty:			break;
		}
	}

	Node(Type type) {
		setType(type);
	}

	~Node() {
	}

};

std::ostream& operator<<(std::ostream& out, Node::Type value);


#endif