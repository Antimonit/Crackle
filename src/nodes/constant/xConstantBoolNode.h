//
// Created by DAVE on 4. 3. 2018.
//

#ifndef CRACKLE_XCONSTANTBOOLNODE_H
#define CRACKLE_XCONSTANTBOOLNODE_H


#include <nodes/xConstantNode.h>

class xConstantBoolNode : public xConstantNode {
public:
	bool boolVal;

	explicit xConstantBoolNode();

	explicit xConstantBoolNode(bool value);

	virtual void defaultValue() override { this->boolVal = false; }

	virtual DataType getType() const override { return typeBool; }

	virtual xConstantNode* clone() const override;

	virtual std::string print() const override { return "Constant Bool"; }

	virtual std::ostream& print(std::ostream& out) const override { return out << (boolVal ? "true" : "false"); }

	virtual xConstantBoolNode* toBool() const override;
};


#endif //CRACKLE_XCONSTANTBOOLNODE_H
