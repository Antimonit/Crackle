//
// Created by DAVE on 4. 3. 2018.
//

#ifndef CRACKLE_XCONSTANTSTRINGNODE_H
#define CRACKLE_XCONSTANTSTRINGNODE_H


#include <nodes/xConstantNode.h>

class xConstantStringNode : public xConstantNode {
public:
	std::string stringVal;

	explicit xConstantStringNode(const std::string& value);

	void defaultValue() override { this->stringVal = ""; }

	DataType getType() const override { return typeString; }

	std::string print() const override { return "Constant String"; }

	std::ostream& print(std::ostream& out) const override { return out << stringVal; }

	xConstantStringNode* toString() override;
};


#endif //CRACKLE_XCONSTANTSTRINGNODE_H
