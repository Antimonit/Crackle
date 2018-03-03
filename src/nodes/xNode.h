//
// Created by DAVE on 7. 2. 2018.
//

#ifndef CRACKLE_XNODE_H
#define CRACKLE_XNODE_H

#include <c++/string>
#include <c++/vector>
#include "Driver.hpp"
#include "node_types.h"


class xNode {
public:
	virtual xNode* ex(MC::Driver* driver) = 0;

	virtual std::string print() const = 0;

	virtual xConstantNode* getValue() {
		return nullptr;
	}

	friend std::ostream& operator<<(std::ostream& os, const xNode& p) {
		return p.print(os);
	}

private:
	virtual std::ostream& print(std::ostream& out) const = 0;

};


#endif //CRACKLE_XNODE_H
