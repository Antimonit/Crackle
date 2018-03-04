//
// Created by DAVE on 7. 2. 2018.
//

#ifndef CRACKLE_XFUNCTIONNODE_H
#define CRACKLE_XFUNCTIONNODE_H

#include "xNode.h"

class xFunctionNode : public xNode {
public:
	std::string name;
	std::vector<xNode*> params;	// actual parameters

	xNode* ex(MC::Driver* driver) override;

	std::string print() const override;

	std::ostream& print(std::ostream& out) const override;};


#endif //CRACKLE_XFUNCTIONNODE_H
