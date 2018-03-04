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

	xVariableDefNode();

//	virtual ~xVariableDefNode();

	xNode* ex(MC::Driver* driver) override;

	xConstantNode* getValue() override;

	std::string print() const override;

	std::ostream& print(std::ostream& out) const override;

};

#endif //CRACKLE_XVARIABLEDEFNODE_H
