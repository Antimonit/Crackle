//
// Created by DAVE on 7. 2. 2018.
//

#ifndef CRACKLE_XVARIABLEDEFNODE_H
#define CRACKLE_XVARIABLEDEFNODE_H

#include "xNode.h"

class xConstantNode;
class xVariableDefNode : public xNode {
public:
	std::string name;
	xConstantNode* value;
	xNode* defaultValue;

	explicit xVariableDefNode(const std::string& name, xConstantNode* value);

	explicit xVariableDefNode(const std::string& name, xConstantNode* value, xNode* defaultValue);

	virtual xNode* ex(MC::Driver* driver) override;

	virtual xConstantNode* getValue() override;

	virtual std::string print() const override;

	virtual std::ostream& print(std::ostream& out) const override;

};

#endif //CRACKLE_XVARIABLEDEFNODE_H
