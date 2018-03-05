//
// Created by DAVE on 7. 2. 2018.
//

#ifndef CRACKLE_XCONSTANTNODE_H
#define CRACKLE_XCONSTANTNODE_H

#include "xNode.h"
#include "DataType.h"


class xConstantIntNode;
class xConstantDoubleNode;
class xConstantBoolNode;
class xConstantStringNode;
class xConstantVoidNode;
class xConstantObjectNode;

class xConstantNode : public xNode {
public:

	static xConstantNode* fromType(DataType type);

	virtual DataType getType() const = 0;

	virtual xConstantIntNode* toInt() { throw "Cannot convert " + typeToString(getType()) + " to type int."; }
	virtual xConstantDoubleNode* toDouble() { throw "Cannot convert " + typeToString(getType()) + " to type double."; }
	virtual xConstantBoolNode* toBool() { throw "Cannot convert " + typeToString(getType()) + " to type bool."; }
	virtual xConstantStringNode* toString() { throw "Cannot convert " + typeToString(getType()) + " to type string."; }
	virtual xConstantVoidNode* toVoid() { throw "Cannot convert " + typeToString(getType()) + " to type void."; }
	virtual xConstantObjectNode* toObject() { throw "Cannot convert " + typeToString(getType()) + " to type object."; }

	virtual void defaultValue() = 0;

	virtual xConstantNode* ex(MC::Driver* driver) override;

	virtual xConstantNode* getValue() override;

};


#endif //CRACKLE_XCONSTANTNODE_H
