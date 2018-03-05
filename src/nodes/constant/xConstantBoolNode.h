//
// Created by DAVE on 4. 3. 2018.
//

#ifndef CRACKLE_XCONSTANTBOOLNODE_H
#define CRACKLE_XCONSTANTBOOLNODE_H


#include <nodes/xConstantNode.h>

class xConstantBoolNode : public xConstantNode {
public:
	bool boolVal;

	explicit xConstantBoolNode(bool value);

	void defaultValue() override { this->boolVal = false; }

	DataType getType() const override { return typeBool; }

	std::string print() const override { return "Constant Bool"; }

	std::ostream& print(std::ostream& out) const override { return out << (boolVal ? "true" : "false"); }

	xConstantBoolNode* toBool() override;
};


#endif //CRACKLE_XCONSTANTBOOLNODE_H
