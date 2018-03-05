//
// Created by DAVE on 7. 2. 2018.
//

#include <parser.tab.hh>
#include <headers/node_helpers.hpp>

#include "xOperatorNode.h"
#include "Driver.hpp"

#include "xEmptyNode.h"
#include "xConstantNode.h"
#include "xReturnNode.h"
#include "xVariableDefNode.h"
#include "xVariableNode.h"
#include "xObjectDefNode.h"
#include "xObjectNode.h"
#include "xFunctionDefNode.h"
#include "xFunctionNode.h"
#include "constant/xConstantIntNode.h"
#include "constant/xConstantDoubleNode.h"
#include "constant/xConstantBoolNode.h"
#include "constant/xConstantStringNode.h"
#include "constant/xConstantVoidNode.h"
#include "constant/xConstantObjectNode.h"

using token = MC::Parser::token;

DataType maxType(xConstantNode* a, xConstantNode* b) const {
	DataType typeA = a->getType();
	DataType typeB = b->getType();
	if (typeA == typeInt && typeB == typeInt) {
		return typeInt;
	} else if (typeA == typeDouble && typeB == typeDouble) {
		return typeDouble;
	} else if ((typeA == typeDouble || typeB == typeDouble) && (typeA == typeInt || typeB == typeInt)) {
		return typeDouble;
	} else if (typeA == typeString && typeB == typeString) {
		return typeString;
	} else if (typeA == typeObject && typeB == typeObject) {
		return typeObject;
	} else {
		return typeUndefined;
	}
}

void widenNode(xConstantNode*& a, DataType maxType) {
	if (a->getType() == maxType) {
		return;
	}
	if (a->getType() == typeInt && maxType == typeDouble) {
		a = a->toDouble();
	} else {
		std::cerr << "Warning: Widening of type " << a->getType()
				  << " to type " << maxType
				  << "." << std::endl;
	}
}

DataType widenNodes(xConstantNode*& a, xConstantNode*& b) {
	DataType max = maxType(a, b);
	widenNode(a, max);
	widenNode(b, max);
	return max;
}



xNode* xOperatorNode::returnx(MC::Driver* driver) {
	if (op.empty()) {
		return new xReturnNode();
	} else {
		xConstantNode* value = op[0]->ex(driver)->getValue();
		return new xReturnNode(value);
	}
}

xNode* xOperatorNode::whilex(MC::Driver* driver) {
	while (op[0]->ex(driver)->getValue()->toBool()->boolVal) {
		driver->pushSymbolTableScope();
		xNode* left = op[1]->ex(driver);
		driver->popSymbolTableScope();
		auto* ret = dynamic_cast<xReturnNode*>(left);
		if (ret != nullptr) {
			return ret;	// TODO: create a new node instead?
		}
	}

	return new xOperatorNode(token::WHILE);
}

xNode* xOperatorNode::forx(MC::Driver* driver) {
	for (op[0]->ex(driver);
		 op[1]->ex(driver)->getValue()->toBool()->boolVal;
		 op[2]->ex(driver)) {

		driver->pushSymbolTableScope();
		xNode* left = op[3]->ex(driver);
		driver->popSymbolTableScope();
		auto* ret = dynamic_cast<xReturnNode*>(left);
		if (ret != nullptr) {
			return ret;	// TODO: create a new node instead?
		}
	}

	return new xOperatorNode(token::FOR);
}

xNode* xOperatorNode::ifx(MC::Driver* driver) {
	driver->pushSymbolTableScope();
	xConstantNode* condition = op[0]->ex(driver)->getValue();

	if (condition->toBool()->boolVal) {
		xNode* left = op[1]->ex(driver);
		auto* ret = dynamic_cast<xReturnNode*>(left);
		if (ret != nullptr) {
			driver->popSymbolTableScope();
			return ret;	// TODO: create a new node instead?
		}
	} else if (op.size() > 2) {
		xNode* right = op[2]->ex(driver);
		auto* ret = dynamic_cast<xReturnNode*>(right);
		if (ret != nullptr) {
			driver->popSymbolTableScope();
			return ret;	// TODO: create a new node instead?
		}
	}

	driver->popSymbolTableScope();
	return new xOperatorNode(token::IF);
}

xNode* xOperatorNode::delimiter(MC::Driver* driver) {
	if (!op.empty()) {
		xNode* left = op[0]->ex(driver);
		auto* ret = dynamic_cast<xReturnNode*>(left);
		if (ret != nullptr) {
			return ret;	// TODO: create a new node instead?
		}
	}
	if (op.size() > 1) {
		xNode* right = op[1]->ex(driver);
		auto* ret = dynamic_cast<xReturnNode*>(right);
		if (ret != nullptr) {
			return ret;	// TODO: create a new node instead?
		}
	}
	return new xOperatorNode(';');
}

xNode* xOperatorNode::dot(MC::Driver* driver) {
	xNode* left = op[0]->ex(driver);
	xConstantNode* value = left->getValue();

	xVariableNode* right = dynamic_cast<xVariableNode*>(op[1]);

	if (value->getType() != typeObject) {
		std::cerr << "Warning: Unable to access '" << right->name
				  << "' in non-object type " << value->getType()
				  << "." << std::endl;
		return new xEmptyNode;
	}
	const xConstantObjectNode* objectValue = value->toObject();
	if (objectValue->objectVal == nullptr) {
		std::cerr << "Warning: Unable to access '" << right->name
				  << "' in null object." << std::endl;
		return new xEmptyNode;
	}

	xObjectNode* object = objectValue->objectVal;
	xObjectDefNode* objectDef = driver->findObject(objectValue->objectTypeName);

	for (size_t i = 0; i < objectDef->vars.size(); i++) {
		xVariableDefNode* varDef = objectDef->vars[i];
		if (varDef->name == right->name) {
			xVariableNode* var = object->vars[i];
			return var;
		}
	}

	std::cerr << "Warning: Unable to access member '" << right->name
			  << "' in object type " << objectValue->objectVal->name
			  << "." << std::endl;

	return new xEmptyNode();
}

xNode* xOperatorNode::assign(MC::Driver* driver) {
//	std::cout << "Left value type " << p->oper.op[0]->getType()
//			  << " " << p->oper.op[0]
//			  << " " << &p->oper.op[0]->variable
//			  << " " << &p->oper.op[0]->variable->value
//			  << std::endl;
//	std::cout << "Left value type ex " << lval->getType()
//			  << " " << lval
//			  << " " << &lval->variable
//			  << " " << &lval->variable->value
//			  << std::endl;
	xNode* lNode = op[0]->ex(driver);

	xVariableNode* lvar = dynamic_cast<xVariableNode*>(lNode);

	if (lvar == nullptr) {
		std::cerr << "Warning: Expression is not assignable." << std::endl;
		return new xEmptyNode;
	}

	xNode* rval = op[1]->ex(driver);
	xConstantNode* rvalue = rval->getValue();

	if (lvar->value->getType() != rvalue->getType()) {
		std::cerr << "Warning: Incompatible assignment of type '" << rvalue->getType()
				  << "' to variable '" << lvar->name
				  << "' of type '" << lvar->value->getType()
				  << "'." << std::endl;
		return new xEmptyNode;
	}

	lvar->value = rvalue;

	return rvalue;
}

xConstantNode* xOperatorNode::uminus(MC::Driver* driver) {
	xConstantNode* value = op[0]->ex(driver)->getValue();

	if (value->getType() == typeInt) {
		return new xConstantIntNode(-value->toInt()->intVal);
	} else if (value->getType() == typeDouble) {
		return new xConstantDoubleNode(-value->toDouble()->doubleVal);
	} else {
		throw "Warning: Invalid argument to unary minus.";
	}
}

xConstantNode* xOperatorNode::plus(MC::Driver* driver) {
	xConstantNode* left = op[0]->ex(driver)->getValue();
	xConstantNode* right = op[1]->ex(driver)->getValue();

	DataType maxType = widenNodes(left, right);
	if (maxType == typeInt) {
		return new xConstantIntNode(left->toInt()->intVal + right->toInt()->intVal);
	} else if (maxType == typeDouble) {
		return new xConstantDoubleNode(left->toDouble()->doubleVal + right->toDouble()->doubleVal);
	} else {
		throw "Warning: Invalid argument to plus operator.";
	}
}

xConstantNode* xOperatorNode::minus(MC::Driver* driver) {
	xConstantNode* left = op[0]->ex(driver)->getValue();
	xConstantNode* right = op[1]->ex(driver)->getValue();

	DataType maxType = widenNodes(left, right);
	if (maxType == typeInt) {
		return new xConstantIntNode(left->toInt()->intVal - right->toInt()->intVal);
	} else if (maxType == typeDouble) {
		return new xConstantDoubleNode(left->toDouble()->doubleVal - right->toDouble()->doubleVal);
	} else {
		throw "Warning: Invalid argument to minus operator.";
	}
}

xConstantNode* xOperatorNode::multiply(MC::Driver* driver) {
	xConstantNode* left = op[0]->ex(driver)->getValue();
	xConstantNode* right = op[1]->ex(driver)->getValue();

	DataType maxType = widenNodes(left, right);
	if (maxType == typeInt) {
		return new xConstantIntNode(left->toInt()->intVal * right->toInt()->intVal);
	} else if (maxType == typeDouble) {
		return new xConstantDoubleNode(left->toDouble()->doubleVal * right->toDouble()->doubleVal);
	} else {
		throw "Warning: Invalid argument to multiply operator.";
	}
}

xConstantNode* xOperatorNode::divide(MC::Driver* driver) {
	xConstantNode* left = op[0]->ex(driver)->getValue();
	xConstantNode* right = op[1]->ex(driver)->getValue();

	DataType maxType = widenNodes(left, right);
	if (maxType == typeInt) {
		return new xConstantIntNode(left->toInt()->intVal / right->toInt()->intVal);
	} else if (maxType == typeDouble) {
		return new xConstantDoubleNode(left->toDouble()->doubleVal / right->toDouble()->doubleVal);
	} else {
		throw "Warning: Invalid argument to divide operator.";
	}
}

xConstantNode* xOperatorNode::modulo(MC::Driver* driver) {
	xConstantNode* left = op[0]->ex(driver)->getValue();
	xConstantNode* right = op[1]->ex(driver)->getValue();

	DataType maxType = widenNodes(left, right);
	if (maxType == typeInt) {
		return new xConstantIntNode(left->toInt()->intVal % right->toInt()->intVal);
	} else {
		throw "Warning: Invalid argument to modulo operator.";
	}
}

xConstantBoolNode* xOperatorNode::andx(MC::Driver* driver) {
	xConstantNode* left = op[0]->ex(driver)->getValue();
	if (left->getType() != typeBool) {
		throw "Warning: Invalid argument of type " + typeToString(left->getType()) + " to AND operator.";
	} else if (!left->toBool()->boolVal) {
		return new xConstantBoolNode(false);
	}

	xConstantNode* right = op[1]->ex(driver)->getValue();
	if (right->getType() != typeBool) {
		throw "Warning: Invalid argument of type " + typeToString(right->getType()) + " to AND operator.";
	} else if (!right->toBool()->boolVal) {
		return new xConstantBoolNode(false);
	}

	return new xConstantBoolNode(true);
}

xConstantBoolNode* xOperatorNode::orx(MC::Driver* driver) {
	xConstantNode* left = op[0]->ex(driver)->getValue();
	if (left->getType() != typeBool) {
		throw "Warning: Invalid argument of type " + typeToString(left->getType()) + " to OR operator.";
	} else if (left->toBool()->boolVal) {
		return new xConstantBoolNode(true);
	}

	xConstantNode* right = op[1]->ex(driver)->getValue();
	if (right->getType() != typeBool) {
		throw "Warning: Invalid argument of type " + typeToString(right->getType()) + " to OR operator.";
	} else if (right->toBool()->boolVal) {
		return new xConstantBoolNode(true);
	}

	return new xConstantBoolNode(false);
}

xConstantBoolNode* xOperatorNode::neg(MC::Driver* driver) {
	xNode* v = op[0]->ex(driver);
	auto* variable = dynamic_cast<xVariableNode*>(v);
	auto* constant = dynamic_cast<xConstantNode*>(v);
	xConstantNode& value = (variable != nullptr) ? *variable->value : *constant;

	if (value.getType() != typeBool) {
		throw "Warning: Invalid argument of type " + typeToString(value.getType()) + " to NEG operator.";
	} else {
		return new xConstantBoolNode(!value.toBool()->boolVal);
	}
}

xConstantNode* xOperatorNode::inc(MC::Driver* driver) {
	xNode* v = op[0]->ex(driver);
	auto* variable = dynamic_cast<xVariableNode*>(v);

	if (variable == nullptr) {
		throw "Warning: Cannot increment non-variable.";
	} else {
		xConstantNode* value = variable->value;
		if (value->getType() == typeInt) {
			return new xConstantIntNode(++value->toInt()->intVal);
		} else if (value->getType() == typeDouble) {
			return new xConstantDoubleNode(++value->toDouble()->doubleVal);
		} else {
			throw "Warning: Cannot increment non-numeric variable.";
		}
	}
}

xConstantNode* xOperatorNode::dec(MC::Driver* driver) {
	xNode* v = op[0]->ex(driver);
	auto* variable = dynamic_cast<xVariableNode*>(v);

	if (variable == nullptr) {
		throw "Warning: Cannot decrement non-variable.";
	} else {
		xConstantNode* value = variable->value;
		if (value->getType() == typeInt) {
			return new xConstantIntNode(--value->toInt()->intVal);
		} else if (value->getType() == typeDouble) {
			return new xConstantDoubleNode(--value->toDouble()->doubleVal);
		} else {
			throw "Warning: Cannot decrement non-numeric variable.";
		}
	}
}

xConstantBoolNode* xOperatorNode::lt(MC::Driver* driver) {
	xNode* l = op[0]->ex(driver);
	xNode* r = op[1]->ex(driver);
	auto* lVariable = dynamic_cast<xVariableNode*>(l);
	auto* rVariable = dynamic_cast<xVariableNode*>(r);
	auto* lConstant = dynamic_cast<xConstantNode*>(l);
	auto* rConstant = dynamic_cast<xConstantNode*>(r);
	xConstantNode& left = (lVariable != nullptr) ? *lVariable->value : *lConstant;
	xConstantNode& right = (rVariable != nullptr) ? *rVariable->value : *rConstant;

	DataType max = maxType(left, right);
	if (max == typeInt) {
		widenNodes(left, right);
		return new xConstantBoolNode(left.toInt()->intVal < right.toInt()->intVal);
	} else if (max == typeDouble) {
		widenNodes(left, right);
		return new xConstantBoolNode(left.toDouble()->doubleVal < right.toDouble()->doubleVal);
	} else {
		throw "Warning: Invalid arguments of types " + typeToString(left.getType())
			  + " and " + typeToString(right.getType())
			  + " to LT operator.";
	}
}

xConstantBoolNode* xOperatorNode::le(MC::Driver* driver) {
	xNode* l = op[0]->ex(driver);
	xNode* r = op[1]->ex(driver);
	auto* lVariable = dynamic_cast<xVariableNode*>(l);
	auto* rVariable = dynamic_cast<xVariableNode*>(r);
	auto* lConstant = dynamic_cast<xConstantNode*>(l);
	auto* rConstant = dynamic_cast<xConstantNode*>(r);
	xConstantNode& left = (lVariable != nullptr) ? *lVariable->value : *lConstant;
	xConstantNode& right = (rVariable != nullptr) ? *rVariable->value : *rConstant;

	DataType max = maxType(left, right);
	if (max == typeInt) {
		widenNodes(left, right);
		return new xConstantBoolNode(left.toInt()->intVal <= right.toInt()->intVal);
	} else if (max == typeDouble) {
		widenNodes(left, right);
		return new xConstantBoolNode(left.toDouble()->doubleVal <= right.toDouble()->doubleVal);
	} else {
		throw "Warning: Invalid arguments of types " + typeToString(left.getType())
			  + " and " + typeToString(right.getType())
			  + " to LE operator.";
	}
}

xConstantBoolNode* xOperatorNode::gt(MC::Driver* driver) {
	xNode* l = op[0]->ex(driver);
	xNode* r = op[1]->ex(driver);
	auto* lVariable = dynamic_cast<xVariableNode*>(l);
	auto* rVariable = dynamic_cast<xVariableNode*>(r);
	auto* lConstant = dynamic_cast<xConstantNode*>(l);
	auto* rConstant = dynamic_cast<xConstantNode*>(r);
	xConstantNode& left = (lVariable != nullptr) ? *lVariable->value : *lConstant;
	xConstantNode& right = (rVariable != nullptr) ? *rVariable->value : *rConstant;

	DataType max = maxType(left, right);
	if (max == typeInt) {
		widenNodes(left, right);
		return new xConstantBoolNode(left.toInt()->intVal > right.toInt()->intVal);
	} else if (max == typeDouble) {
		widenNodes(left, right);
		return new xConstantBoolNode(left.toDouble()->doubleVal > right.toDouble()->doubleVal);
	} else {
		throw "Warning: Invalid arguments of types " + typeToString(left.getType())
			  + " and " + typeToString(right.getType())
			  + " to GT operator.";
	}
}

xConstantBoolNode* xOperatorNode::ge(MC::Driver* driver) {
	xNode* l = op[0]->ex(driver);
	xNode* r = op[1]->ex(driver);
	auto* lVariable = dynamic_cast<xVariableNode*>(l);
	auto* rVariable = dynamic_cast<xVariableNode*>(r);
	auto* lConstant = dynamic_cast<xConstantNode*>(l);
	auto* rConstant = dynamic_cast<xConstantNode*>(r);
	xConstantNode& left = (lVariable != nullptr) ? *lVariable->value : *lConstant;
	xConstantNode& right = (rVariable != nullptr) ? *rVariable->value : *rConstant;

	DataType max = maxType(left, right);
	if (max == typeInt) {
		widenNodes(left, right);
		return new xConstantBoolNode(left.toInt()->intVal >= right.toInt()->intVal);
	} else if (max == typeDouble) {
		widenNodes(left, right);
		return new xConstantBoolNode(left.toDouble()->doubleVal >= right.toDouble()->doubleVal);
	} else {
		throw "Warning: Invalid arguments of types " + typeToString(left.getType())
			  + " and " + typeToString(right.getType())
			  + " to GE operator.";
	}
}

xConstantBoolNode* xOperatorNode::eq(MC::Driver* driver) {
	xNode* l = op[0]->ex(driver);
	xNode* r = op[1]->ex(driver);
	auto* lVariable = dynamic_cast<xVariableNode*>(l);
	auto* rVariable = dynamic_cast<xVariableNode*>(r);
	auto* lConstant = dynamic_cast<xConstantNode*>(l);
	auto* rConstant = dynamic_cast<xConstantNode*>(r);
	xConstantNode& left = (lVariable != nullptr) ? *lVariable->value : *lConstant;
	xConstantNode& right = (rVariable != nullptr) ? *rVariable->value : *rConstant;

	DataType max = maxType(left, right);
	if (max == typeInt) {
		widenNodes(left, right);
		return new xConstantBoolNode(left.toInt()->intVal == right.toInt()->intVal);
	} else if (max == typeDouble) {
		widenNodes(left, right);
		return new xConstantBoolNode(left.toDouble()->doubleVal == right.toDouble()->doubleVal);
	} else if (max == typeObject) {
		widenNodes(left, right);
		return new xConstantBoolNode(left.toObject()->objectVal == right.toObject()->objectVal);
	} else {
		throw "Warning: Invalid arguments of types " + typeToString(left.getType())
			  + " and " + typeToString(right.getType())
			  + " to EQ operator.";
	}
}

xConstantBoolNode* xOperatorNode::ne(MC::Driver* driver) {
	xNode* l = op[0]->ex(driver);
	xNode* r = op[1]->ex(driver);
	auto* lVariable = dynamic_cast<xVariableNode*>(l);
	auto* rVariable = dynamic_cast<xVariableNode*>(r);
	auto* lConstant = dynamic_cast<xConstantNode*>(l);
	auto* rConstant = dynamic_cast<xConstantNode*>(r);
	xConstantNode& left = (lVariable != nullptr) ? *lVariable->value : *lConstant;
	xConstantNode& right = (rVariable != nullptr) ? *rVariable->value : *rConstant;

	DataType max = maxType(left, right);
	if (max == typeInt) {
		widenNodes(left, right);
		return new xConstantBoolNode(left.toInt()->intVal != right.toInt()->intVal);
	} else if (max == typeDouble) {
		widenNodes(left, right);
		return new xConstantBoolNode(left.toDouble()->doubleVal != right.toDouble()->doubleVal);
	} else if (max == typeObject) {
		widenNodes(left, right);
		return new xConstantBoolNode(left.toObject()->objectVal != right.toObject()->objectVal);
	} else {
		throw "Warning: Invalid arguments of types " + typeToString(left.getType())
				  + " and " + typeToString(right.getType())
				  + " to NE operator.";
	}
}

xNode* xOperatorNode::printx(MC::Driver* driver) {
	xNode* value = op[0]->ex(driver);

	xOperatorNode* operatorNode = dynamic_cast<xOperatorNode*>(value);
	xConstantNode* constantNode = dynamic_cast<xConstantNode*>(value);
	xVariableNode* variableNode = dynamic_cast<xVariableNode*>(value);
	xVariableDefNode* variableDefNode = dynamic_cast<xVariableDefNode*>(value);
	xFunctionNode* functionNode = dynamic_cast<xFunctionNode*>(value);
	xFunctionDefNode* functionDefNode = dynamic_cast<xFunctionDefNode*>(value);
	xObjectNode* objectNode = dynamic_cast<xObjectNode*>(value);
	xObjectDefNode* objectDefNode = dynamic_cast<xObjectDefNode*>(value);
	xEmptyNode* emptyNode = dynamic_cast<xEmptyNode*>(value);

	if (operatorNode != nullptr) {
		switch (operatorNode->oper) {
			case token::FOR:
				*driver->out << "FOR";
				break;
			case token::WHILE:
				*driver->out << "WHILE";
				break;
			case token::IF:
				*driver->out << "IF";
				break;
			case token::FUN:
				*driver->out << "FUN";
				break;
			case token::VAR:
				*driver->out << "VAR";
				break;
			default:
				*driver->out << "WRONG OPERATOR";
		}
	} else if (constantNode != nullptr) {
		*driver->out << *constantNode;
	} else if (variableNode != nullptr) {
		*driver->out << *variableNode;
	} else if (variableDefNode != nullptr) {
		*driver->out << *variableDefNode;
	} else if (functionDefNode != nullptr) {
		*driver->out << "Function " << functionDefNode->name << " defined";
	} else if (emptyNode != nullptr) {
		*driver->out << "EMPTY";
	} else {
		*driver->out << "WRONG TYPE";
	}

	return new xEmptyNode;
}

xNode* xOperatorNode::println(MC::Driver* driver) {
	auto result = printx(driver);
	*driver->out << std::endl;
	return result;
}


xNode* xOperatorNode::ex(MC::Driver* driver) {
	switch (oper) {
		case token::PRINT:	return printx(driver);
		case token::PRINTLN:return println(driver);
		case token::RETURN:	return returnx(driver);

		case token::WHILE:	return whilex(driver);
		case token::IF:		return ifx(driver);
		case token::FOR:	return forx(driver);
		case ';':			return delimiter(driver);
		case '.':			return dot(driver);
		case '=':			return assign(driver);

		case token::UMINUS:	return uminus(driver);

		case '+':			return plus(driver);
		case '-':			return minus(driver);
		case '*':			return multiply(driver);
		case '/':			return divide(driver);
		case '%':			return modulo(driver);

		case token::AND:	return andx(driver);
		case token::OR:		return orx(driver);
		case token::NEG:	return neg(driver);

		case token::INC:	return inc(driver);
		case token::DEC:	return dec(driver);

		case token::LT:		return lt(driver);
		case token::LE:		return le(driver);
		case token::GT:		return gt(driver);
		case token::GE:		return ge(driver);
		case token::EQ:		return eq(driver);
		case token::NE:		return ne(driver);

		default:
			std::cerr << "Error: unknown operator '" << oper << "'." << std::endl;
			return nullptr;
	}
}

std::string xOperatorNode::print() const {
	return "Operator";
}

std::ostream& xOperatorNode::print(std::ostream& out) const {
	return out << static_cast<char>(oper) << " "
			   << op.size() << " operands";
}

