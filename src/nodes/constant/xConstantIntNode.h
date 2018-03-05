//
// Created by DAVE on 4. 3. 2018.
//

#ifndef CRACKLE_XCONSTANTINTNODE_H
#define CRACKLE_XCONSTANTINTNODE_H


#include <nodes/xConstantNode.h>

class xConstantIntNode : public xConstantNode {
public:
	int intVal;

	explicit xConstantIntNode();

	explicit xConstantIntNode(int value);

	virtual void defaultValue() override { this->intVal = 0; }

	virtual DataType getType() const override { return typeInt; }

	virtual xConstantNode* clone() const override;

	virtual std::string print() const override { return "Constant Int"; }

	virtual std::ostream& print(std::ostream& out) const override { return out << intVal; }

	virtual xConstantIntNode* toInt() const override;

	virtual xConstantDoubleNode* toDouble() const override;
};


#endif //CRACKLE_XCONSTANTINTNODE_H
