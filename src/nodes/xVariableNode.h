//
// Created by DAVE on 7. 2. 2018.
//

#ifndef CRACKLE_XVARIABLENODE_H
#define CRACKLE_XVARIABLENODE_H

#include "xNode.h"

class xVariableNode : public xNode {
public:
	std::string name;
	xConstantNode* value;

	xVariableNode(const std::string& name, xConstantNode* value);

	virtual xNode* ex(MC::Driver* driver) override;

	virtual xConstantNode* getValue() override;

	virtual std::string print() const override;

	virtual std::ostream& print(std::ostream& out) const override;

};


#endif //CRACKLE_XVARIABLENODE_H
