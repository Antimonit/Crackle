//
// Created by DAVE on 7. 2. 2018.
//

#ifndef CRACKLE_XOPERATORNODE_H
#define CRACKLE_XOPERATORNODE_H


#include <c++/vector>
#include "xNode.h"

class xOperatorNode : xNode {
public:
	int oper;
	std::vector<xNode*> op;

	explicit xOperatorNode(int val) {
		oper = val;
	}

	xNode* ex(MC::Driver* driver) override;

	std::string print() const override {
		return "Operator";
	}

	std::ostream& print(std::ostream& out) const override {
		return out << const_cast<char>(oper) << " "
				   << op.size() << " operands";
	}

private:

	xNode* returnx(MC::Driver* driver);
	xNode* whilex(MC::Driver* driver);
	xNode* forx(MC::Driver* driver);
	xNode* ifx(MC::Driver* driver);
	xNode* delimiter(MC::Driver* driver);
	xNode* dot(MC::Driver* driver);
	xNode* assign(MC::Driver* driver);
	xNode* uminus(MC::Driver* driver);
	xNode* plus(MC::Driver* driver);
	xNode* minus(MC::Driver* driver);
	xNode* multiply(MC::Driver* driver);
	xNode* divide(MC::Driver* driver);
	xNode* modulo(MC::Driver* driver);
	xNode* andx(MC::Driver* driver);
	xNode* orx(MC::Driver* driver);
	xNode* neg(MC::Driver* driver);
	xNode* inc(MC::Driver* driver);
	xNode* dec(MC::Driver* driver);
	xNode* lt(MC::Driver* driver);
	xNode* le(MC::Driver* driver);
	xNode* gt(MC::Driver* driver);
	xNode* ge(MC::Driver* driver);
	xNode* eq(MC::Driver* driver);
	xNode* ne(MC::Driver* driver);
	xNode* printx(MC::Driver* driver);
	xNode* println(MC::Driver* driver);

};


#endif //CRACKLE_XOPERATORNODE_H
