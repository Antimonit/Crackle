//
// Created by DAVE on 4. 3. 2018.
//

#ifndef CRACKLE_XCONSTANTVOIDNODE_H
#define CRACKLE_XCONSTANTVOIDNODE_H


#include <nodes/xConstantNode.h>

class xConstantVoidNode : public xConstantNode {
public:
	// no value

	explicit xConstantVoidNode();

	void defaultValue() override { /* do nothing */ }

	virtual DataType getType() const override { return typeVoid; }

	virtual xConstantNode* clone() const override;

	virtual std::string print() const override { return "Constant Void"; }

	virtual std::ostream& print(std::ostream& out) const override { return out << "void"; }

	virtual xConstantVoidNode* toVoid() const override;

};


#endif //CRACKLE_XCONSTANTVOIDNODE_H
