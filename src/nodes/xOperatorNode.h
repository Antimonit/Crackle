//
// Created by DAVE on 7. 2. 2018.
//

#ifndef CRACKLE_XOPERATORNODE_H
#define CRACKLE_XOPERATORNODE_H


#include "xNode.h"

class xOperatorNode : public xNode {
public:
	int oper;
	std::vector<xNode*> op;

	explicit xOperatorNode(int val);

	virtual xNode* ex(MC::Driver* driver) override;

	virtual std::string print() const override;

	virtual std::ostream& print(std::ostream& out) const override;

private:

	xNode* returnx(MC::Driver* driver);
	xNode* whilex(MC::Driver* driver);
	xNode* forx(MC::Driver* driver);
	xNode* ifx(MC::Driver* driver);
	xNode* delimiter(MC::Driver* driver);
	xNode* dot(MC::Driver* driver);
	xNode* assign(MC::Driver* driver);
	xConstantNode* uminus(MC::Driver* driver);
	xConstantNode* plus(MC::Driver* driver);
	xConstantNode* minus(MC::Driver* driver);
	xConstantNode* multiply(MC::Driver* driver);
	xConstantNode* divide(MC::Driver* driver);
	xConstantNode* modulo(MC::Driver* driver);
	xConstantBoolNode* andx(MC::Driver* driver);
	xConstantBoolNode* orx(MC::Driver* driver);
	xConstantBoolNode* neg(MC::Driver* driver);
	xConstantNode* inc(MC::Driver* driver);
	xConstantNode* dec(MC::Driver* driver);
	xConstantBoolNode* lt(MC::Driver* driver);
	xConstantBoolNode* le(MC::Driver* driver);
	xConstantBoolNode* gt(MC::Driver* driver);
	xConstantBoolNode* ge(MC::Driver* driver);
	xConstantBoolNode* eq(MC::Driver* driver);
	xConstantBoolNode* ne(MC::Driver* driver);
	xNode* printx(MC::Driver* driver);
	xNode* println(MC::Driver* driver);

};


#endif //CRACKLE_XOPERATORNODE_H
