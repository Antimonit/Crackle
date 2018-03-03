//
// Created by DAVE on 7. 2. 2018.
//

#ifndef CRACKLE_XRETURNNODE_H
#define CRACKLE_XRETURNNODE_H


#include "xNode.h"
#include "xConstantNode.h"

class xReturnNode : xNode {
public:
	xConstantNode* value;

	xReturnNode() {
		this->value = new xConstantNode(typeVoid);
	}

	explicit xReturnNode(xConstantNode* value) {
		this->value = value;
	}

	xNode* ex(MC::Driver* driver) override {
		return this;
	}

};


#endif //CRACKLE_XRETURNNODE_H
