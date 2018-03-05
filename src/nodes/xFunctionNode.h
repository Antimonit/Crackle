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

	virtual xNode* ex(MC::Driver* driver) override;

	virtual std::string print() const override;

	virtual std::ostream& print(std::ostream& out) const override;};


#endif //CRACKLE_XFUNCTIONNODE_H
