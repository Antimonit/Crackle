//
// Created by DAVE on 7. 2. 2018.
//

#ifndef CRACKLE_XRETURNNODE_H
#define CRACKLE_XRETURNNODE_H

#include "xNode.h"

class xConstantNode;

class xReturnNode : public xNode {
public:
	xConstantNode* value;

	xReturnNode();

	explicit xReturnNode(xConstantNode* value);

	xNode* ex(MC::Driver* driver) override;

	std::string print() const override;

private:
	std::ostream& print(std::ostream& out) const override;
};


#endif //CRACKLE_XRETURNNODE_H
