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

	virtual void defaultValue() override { this->doubleVal = 0.0; }

	virtual DataType getType() const override { return typeDouble; }

	virtual xConstantNode* clone() const override;

	virtual std::string print() const override { return "Constant Double"; }

	virtual std::ostream& print(std::ostream& out) const override { return out << doubleVal; }

	virtual xConstantIntNode* toInt() const override;

	virtual xConstantDoubleNode* toDouble() const override;
};


#endif //CRACKLE_XCONSTANTDOUBLENODE_H
