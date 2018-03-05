//
// Created by DAVE on 4. 3. 2018.
//

#ifndef CRACKLE_XCONSTANTOBJECTNODE_H
#define CRACKLE_XCONSTANTOBJECTNODE_H


#include <nodes/xConstantNode.h>
#include <nodes/xObjectNode.h>

class xConstantObjectNode : public xConstantNode {
public:
	xObjectNode* objectVal;
	std::string objectTypeName;

	explicit xConstantObjectNode(xObjectNode* value, const std::string& typeName);

	void defaultValue() override {
		this->objectVal = nullptr;
		this->objectTypeName = "null";
	}

	DataType getType() const override { return typeObject; }

	std::string print() const override { return "Constant Object"; }

	std::ostream& print(std::ostream& out) const override {
		if (objectVal == nullptr)
			return out << "null";
		else
			return out << "<object " << objectTypeName << ">";
	}

	xConstantObjectNode* toObject() override;
};


#endif //CRACKLE_XCONSTANTOBJECTNODE_H
