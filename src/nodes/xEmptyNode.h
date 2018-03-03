//
// Created by DAVE on 7. 2. 2018.
//

#ifndef CRACKLE_XEMPTYNODE_H
#define CRACKLE_XEMPTYNODE_H


#include "Driver.hpp"
#include "xNode.h"

class xEmptyNode : xNode {

	xNode* ex(MC::Driver* driver) override {
		return this;
	}

	std::string print() const override {
		return "EMPTY";
	}

	std::ostream& print(std::ostream& out) const override {
		return out ;
	}

};


#endif //CRACKLE_XEMPTYNODE_H
