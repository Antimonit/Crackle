//
// Created by DAVE on 7. 2. 2018.
//

#ifndef CRACKLE_XCONSTANTNODE_H
#define CRACKLE_XCONSTANTNODE_H

#include "xNode.h"
#include "DataType.h"

class xObjectNode;

class xConstantNode : public xNode {
	DataType dataType {typeUndefined};

	void copyConstantNode(const xConstantNode& other) {
		setType(other.dataType);
		switch (dataType) {
			case typeInt:		intVal = other.intVal; break;
			case typeDouble:	doubleVal = other.doubleVal; break;
			case typeBool:		boolVal = other.boolVal; break;
			case typeString:	stringVal = other.stringVal; break;
			case typeVoid:		break;
			case typeObject:
				objectVal = other.objectVal;
				objectTypeName = other.objectTypeName;
				break;
			case typeUndefined:
				break;
		}
	}

public:
	explicit xConstantNode(DataType type) {
		setType(type);
		defaultValue();
	}

	explicit xConstantNode(int value) {
		setType(typeInt);
		intVal = value;
	}
	explicit xConstantNode(double value) {
		setType(typeDouble);
		doubleVal = value;
	}
	explicit xConstantNode(bool value) {
		setType(typeBool);
		boolVal = value;
	}
	explicit xConstantNode(std::string value) {
		setType(typeString);
		stringVal = value;
	}
	explicit xConstantNode(xObjectNode* value, std::string typeName) {
		setType(typeObject);
		objectVal = value;
		objectTypeName = typeName;
	}

	union {
		int intVal;
		double doubleVal;
		bool boolVal;
		std::string stringVal;
		xObjectNode* objectVal;
	};
	std::string objectTypeName;

	DataType getType() const {
		return dataType;
	}

	void setType(DataType type) {
		this->dataType = type;
		switch (type) {
			case typeString: 	new ((void*) &stringVal) std::string; break;
			default:			break;
		}
	}

	void defaultValue() {
		if (getType() == typeInt) {
			intVal = 0;
		} else if (getType() == typeDouble) {
			doubleVal = 0.0;
		} else if (getType() == typeString) {
			stringVal = "";
		} else if (getType() == typeBool) {
			boolVal = false;
		} else if (getType() == typeObject) {
			objectVal = nullptr;
			objectTypeName = "null";
		}
	}

	xNode* ex(MC::Driver* driver) override;

	xConstantNode* getValue() override;

	std::string print() const override;

	std::ostream& print(std::ostream& out) const override;

};


#endif //CRACKLE_XCONSTANTNODE_H
