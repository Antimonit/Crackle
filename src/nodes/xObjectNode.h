//
// Created by DAVE on 7. 2. 2018.
//

#ifndef CRACKLE_XOBJECTNODE_H
#define CRACKLE_XOBJECTNODE_H

#include "xNode.h"

class xVariableNode;
class xObjectNode : public xNode {
public:
	bool marked;
	std::string name;
	std::vector<xVariableNode*> vars;

	virtual xNode* ex(MC::Driver* driver) override;

	virtual std::string print() const override;

	virtual std::ostream& print(std::ostream& out) const override;

	virtual ~xObjectNode() {

	}
};


#endif //CRACKLE_XOBJECTNODE_H
