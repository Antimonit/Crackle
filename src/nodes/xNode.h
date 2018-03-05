//
// Created by DAVE on 7. 2. 2018.
//

#ifndef CRACKLE_XNODE_H
#define CRACKLE_XNODE_H

#include <c++/string>
#include <c++/vector>

namespace MC { // xNode requires
	class Driver;
	class Scanner;
	class Parser;
}

class xConstantNode;
class xNode {
public:

	virtual xNode* ex(MC::Driver* driver) = 0;

	virtual std::string print() const = 0;

	virtual xConstantNode* getValue() {
		throw "Node does not contain a constant node.";
	}

	friend std::ostream& operator<<(std::ostream& os, const xNode& node) {
		return node.print(os);
	}

private:
	virtual std::ostream& print(std::ostream& out) const = 0;

};


#endif //CRACKLE_XNODE_H
