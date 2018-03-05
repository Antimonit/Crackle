//
// Created by DAVE on 7. 2. 2018.
//

#ifndef CRACKLE_XFUNCTIONDEFNODE_H
#define CRACKLE_XFUNCTIONDEFNODE_H

#include "xNode.h"
#include "DataType.h"

class xVariableDefNode;
class xFunctionDefNode : public xNode {
public:
	std::string name;
	DataType dataType;
	xNode* root;
	std::vector<xVariableDefNode*> params;	// formal parameters

	virtual xNode* ex(MC::Driver* driver) override;

	virtual std::string print() const override;

	virtual std::ostream& print(std::ostream& out) const override;
};


#endif //CRACKLE_XFUNCTIONDEFNODE_H
