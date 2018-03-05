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

	explicit xConstantObjectNode();

	explicit xConstantObjectNode(xObjectNode* value, const std::string& typeName);

	virtual void defaultValue() override {
		this->objectVal = nullptr;
		this->objectTypeName = "null";
	}

	virtual DataType getType() const override { return typeObject; }

	virtual xConstantNode* clone() const override;

	virtual std::string print() const override { return "Constant Object"; }

	virtual std::ostream& print(std::ostream& out) const override {
		if (objectVal == nullptr)
			return out << "null";
		else
			return out << "<object " << objectTypeName << ">";
	}

	virtual xConstantObjectNode* toObject() const override;
};


#endif //CRACKLE_XCONSTANTOBJECTNODE_H
