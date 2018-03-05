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

	explicit xReturnNode();

	explicit xReturnNode(xConstantNode* value);

	virtual xNode* ex(MC::Driver* driver) override;

	virtual xConstantNode* getValue() override;

	virtual std::string print() const override;

	virtual std::ostream& print(std::ostream& out) const override;
};


#endif //CRACKLE_XRETURNNODE_H
