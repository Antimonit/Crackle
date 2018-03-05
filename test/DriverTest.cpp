//
// Created by DAVE on 3. 2. 2018.
//

#include "DriverTest.hpp"

void checkResultIsConstant(xNode* result, const std::string& name, int value) {
	auto* constant = dynamic_cast<xConstantNode*>(result);
	if (constant == nullptr) throw "Result of node " + name + " is not of type Constant.";
	if (constant->getType() != typeInt) throw "Result of node " + name + " is not of type Constant " + typeInt + ".";
	if (constant->toInt()->intVal != value) throw "Result of node " + name + " is not correct.";
}
void checkResultIsConstant(xNode* result, const std::string& name, bool value) {
	auto* constant = dynamic_cast<xConstantNode*>(result);
	if (constant == nullptr) throw "Result of node " + name + " is not of type Constant.";
	if (constant->getType() != typeBool) throw "Result of node " + name + " is not of type Constant " + typeBool + ".";
	if (constant->toBool()->boolVal != value) throw "Result of node " + name + " is not correct.";
}
void checkResultIsConstant(xNode* result, const std::string& name, double value) {
	auto* constant = dynamic_cast<xConstantNode*>(result);
	if (constant == nullptr) throw "Result of node " + name + " is not of type Constant.";
	if (constant->getType() != typeDouble) throw "Result of node " + name + " is not of type Constant " + typeDouble + ".";
	if (constant->toDouble()->doubleVal != value) throw "Result of node " + name + " is not correct.";
}
void checkResultIsConstant(xNode* result, const std::string& name, const std::string& value) {
	auto* constant = dynamic_cast<xConstantNode*>(result);
	if (constant == nullptr) throw "Result of node " + name + " is not of type Constant.";
	if (constant->getType() != typeString) throw "Result of node " + name + " is not of type Constant " + typeString + ".";
	if (constant->toString()->stringVal != value) throw "Result of node " + name + " is not correct.";
}
void checkResultIsConstantUndefined(xNode* result, const std::string& name) {
	auto* constant = dynamic_cast<xConstantNode*>(result);
	if (constant == nullptr) throw "Result of node " + name + " is not of type Constant.";
	if (constant->getType() != typeUndefined) throw "Result of node " + name + " is not of type Constant " + typeUndefined + ".";
}

xNode* MC::DriverTest::testPLUS(xConstantNode* operandOne, xConstantNode* operandTwo) {
	std::istringstream in;
	std::ostringstream out;
	Driver driver = MC::Driver::Builder().input(in).output(out).build();

	xOperatorNode* node = operatorNode('+', operandOne, operandTwo);
	xNode* result = node->ex(&driver);

//	delete node;
//	delete operandTwo;
//	delete operandOne;

	return result;
}
void MC::DriverTest::testPLUSIntInt() {
	xConstantNode* operandOne = new xConstantIntNode(5);
	xConstantNode* operandTwo = new xConstantIntNode(5);
	xNode* result = testPLUS(operandOne, operandTwo);

	checkResultIsConstant(result, "PLUS", 10);
}
void MC::DriverTest::testPLUSDoubleDouble() {
	xConstantNode* operandOne = new xConstantDoubleNode(5.1);
	xConstantNode* operandTwo = new xConstantDoubleNode(5.2);
	xNode* result = testPLUS(operandOne, operandTwo);

	checkResultIsConstant(result, "PLUS", 10.3);
}
void MC::DriverTest::testPLUSIntDouble() {
	xConstantNode* operandOne = new xConstantIntNode(5);
	xConstantNode* operandTwo = new xConstantDoubleNode(5.0);
	xNode* result = testPLUS(operandOne, operandTwo);

	checkResultIsConstant(result, "PLUS", 10.0);
}
void MC::DriverTest::testPLUSIntBoolean() {
	xConstantNode* operandOne = new xConstantIntNode(5);
	xConstantNode* operandTwo = new xConstantBoolNode(false);
	xNode* result = testPLUS(operandOne, operandTwo);

	checkResultIsConstantUndefined(result, "PLUS");
}

void MC::DriverTest::testEQ() {
	std::istringstream in;
	std::ostringstream out;
	Driver driver = MC::Driver::Builder().input(in).output(out).build();

	xConstantNode* operandOne = new xConstantIntNode(5);
	xConstantNode* operandTwo = new xConstantIntNode(5);
	xOperatorNode* node = operatorNode(MC::Parser::token::EQ, operandOne, operandTwo);
	xNode* result = node->ex(&driver);

	checkResultIsConstant(result, "EQ", true);

//	delete node;
//	delete operandTwo;
//	delete operandOne;
}

void MC::DriverTest::testINC() {
	std::istringstream in;
	std::ostringstream out;
	Driver driver = MC::Driver::Builder().input(in).output(out).build();

	xVariableNode* var = variable("a", 5);
	driver.addVariable(var);

	xVariableNode* operandOne = variable("a");
	xOperatorNode* node = operatorNode(MC::Parser::token::INC, operandOne);
	xNode* result = node->ex(&driver);

	checkResultIsConstant(result, "INC", 6);

//	delete node;
//	delete operandOne;
//	delete var;
}
