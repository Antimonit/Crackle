//
// Created by DAVE on 4. 3. 2018.
//

#ifndef CRACKLE_XCONSTANTSTRINGNODE_H
#define CRACKLE_XCONSTANTSTRINGNODE_H


#include <nodes/xConstantNode.h>

class xConstantStringNode : public xConstantNode {
public:
	std::string stringVal;

	explicit xConstantStringNode();

	explicit xConstantStringNode(const std::string& value);

	virtual void defaultValue() override { this->stringVal = ""; }

	virtual DataType getType() const override { return typeString; }

	virtual xConstantNode* clone() const override;

	virtual std::string print() const override { return "Constant String"; }

	virtual std::ostream& print(std::ostream& out) const override { return out << stringVal; }

	virtual xConstantStringNode* toString() const override;
};


#endif //CRACKLE_XCONSTANTSTRINGNODE_H
