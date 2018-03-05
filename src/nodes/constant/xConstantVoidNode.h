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

	DataType getType() const override { return typeVoid; }

	std::string print() const override { return "Constant Void"; }

	std::ostream& print(std::ostream& out) const override { return out << "void"; }

	xConstantVoidNode* toVoid() override;

};


#endif //CRACKLE_XCONSTANTVOIDNODE_H
