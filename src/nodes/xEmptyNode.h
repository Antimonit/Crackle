//
// Created by DAVE on 7. 2. 2018.
//

#ifndef CRACKLE_XEMPTYNODE_H
#define CRACKLE_XEMPTYNODE_H

#include "xNode.h"

class xEmptyNode : public xNode {

	xNode* ex(MC::Driver* driver) override;

	std::string print() const override;

	std::ostream& print(std::ostream& out) const override;

};


#endif //CRACKLE_XEMPTYNODE_H
