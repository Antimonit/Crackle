//
// Created by DAVE on 4. 3. 2018.
//

#ifndef CRACKLE_XCONSTANTDOUBLENODE_H
#define CRACKLE_XCONSTANTDOUBLENODE_H


#include <nodes/xConstantNode.h>

class xConstantDoubleNode : public xConstantNode {
public:
	double doubleVal;

	explicit xConstantDoubleNode();

	explicit xConstantDoubleNode(double value);

	void defaultValue() override { this->doubleVal = 0.0; }

	DataType getType() const override { return typeDouble; }

	std::string print() const override { return "Constant Double"; }

	std::ostream& print(std::ostream& out) const override { return out << doubleVal; }

	xConstantIntNode* toInt() override;

	xConstantDoubleNode* toDouble() override;
};


#endif //CRACKLE_XCONSTANTDOUBLENODE_H
