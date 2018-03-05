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

	virtual xConstantNode* clone() const = 0;

	virtual xConstantIntNode* toInt()		const { throw "Cannot convert " + typeToString(getType()) + " to type int."; }
	virtual xConstantDoubleNode* toDouble() const { throw "Cannot convert " + typeToString(getType()) + " to type double."; }
	virtual xConstantBoolNode* toBool()		const { throw "Cannot convert " + typeToString(getType()) + " to type bool."; }
	virtual xConstantStringNode* toString()	const { throw "Cannot convert " + typeToString(getType()) + " to type string."; }
	virtual xConstantObjectNode* toObject()	const { throw "Cannot convert " + typeToString(getType()) + " to type object."; }
	virtual xConstantVoidNode* toVoid()		const { throw "Cannot convert " + typeToString(getType()) + " to type void."; }

	virtual void defaultValue() = 0;

	virtual xConstantNode* ex(MC::Driver* driver) override;

	virtual xConstantNode* getValue() override;

};


#endif //CRACKLE_XCONSTANTNODE_H
