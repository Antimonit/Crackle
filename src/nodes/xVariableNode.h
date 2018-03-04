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

	xVariableNode();

	xNode* ex(MC::Driver* driver) override;

	std::string print() const override;

	std::ostream& print(std::ostream& out) const override;

};


#endif //CRACKLE_XVARIABLENODE_H
