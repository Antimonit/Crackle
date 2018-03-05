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

	void defaultValue() override { this->intVal = 0; }

	DataType getType() const override { return typeInt; }

	std::string print() const override { return "Constant Int"; }

	std::ostream& print(std::ostream& out) const override { return out << intVal; }

	xConstantIntNode* toInt() override;

	xConstantDoubleNode* toDouble() override;
};


#endif //CRACKLE_XCONSTANTINTNODE_H
