//
// Created by DAVE on 7. 2. 2018.
//

#ifndef CRACKLE_XOBJECTDEFNODE_H
#define CRACKLE_XOBJECTDEFNODE_H

#include "xNode.h"

class xVariableDefNode;

class xObjectDefNode : public xNode {
public:
	std::string name;
	std::vector<xVariableDefNode*> vars;

	xNode* ex(MC::Driver* driver) override;

	std::string print() const override;

	std::ostream& print(std::ostream& out) const override;

};


#endif //CRACKLE_XOBJECTDEFNODE_H
