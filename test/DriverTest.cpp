//
// Created by DAVE on 3. 2. 2018.
//

#include "DriverTest.hpp"

void checkResultIsConstant(xNode* result, const std::string& name, int value) {
	auto* constant = dynamic_cast<xConstantNode*>(result);
	if (constant == nullptr) throw "Result of node " + name + " is not of type Constant.";
	if (constant->getType() != typeInt) throw "Result of node " + name + " is not of type Constant " + typeInt + ".";
	if (constant->toInt()->intVal != value) throw "Result of node " + name + " is not correct. Expected " + std::to_string(value) + ", actual " + std::to_string(constant->toInt()->intVal) + ".";
}
void checkResultIsConstant(xNode* result, const std::string& name, bool value) {
	auto* constant = dynamic_cast<xConstantNode*>(result);
	if (constant == nullptr) throw "Result of node " + name + " is not of type Constant.";
	if (constant->getType() != typeBool) throw "Result of node " + name + " is not of type Constant " + typeBool + ".";
	if (constant->toBool()->boolVal != value) throw "Result of node " + name + " is not correct. Expected " + std::to_string(value) + ", actual " + std::to_string(constant->toBool()->boolVal) + ".";
}
void checkResultIsConstant(xNode* result, const std::string& name, double value) {
	auto* constant = dynamic_cast<xConstantNode*>(result);
	if (constant == nullptr) throw "Result of node " + name + " is not of type Constant.";
	if (constant->getType() != typeDouble) throw "Result of node " + name + " is not of type Constant " + typeDouble + ".";
	if (constant->toDouble()->doubleVal != value) throw "Result of node " + name + " is not correct. Expected " + std::to_string(value) + ", actual " + std::to_string(constant->toDouble()->doubleVal) + ".";
}
void checkResultIsConstant(xNode* result, const std::string& name, const std::string& value) {
	auto* constant = dynamic_cast<xConstantNode*>(result);
	if (constant == nullptr) throw "Result of node " + name + " is not of type Constant.";
	if (constant->getType() != typeString) throw "Result of node " + name + " is not of type Constant " + typeString + ".";
	if (constant->toString()->stringVal != value) throw "Result of node " + name + " is not correct. Expected " + value + ", actual " + constant->toString()->stringVal + ".";
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

	try {
		testPLUS(operandOne, operandTwo);
	} catch (const char* e) {
		std::cout << "Exception caught: " << e << std::endl;	// TODO: catch exception silently
		return;
	} catch (const std::string& e) {
		std::cout << "Exception caught: " << e << std::endl;
		return;
	}
	throw "Exception expected.";
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

	xVariableNode* operand = variable("a");
	xOperatorNode* node = operatorNode(MC::Parser::token::INC, operand);
	xNode* result = node->ex(&driver);

	checkResultIsConstant(result, "INC", 6);

	xVariableNode* var2 = driver.findVariable("a");
	int variableValue = var2->value->toInt()->intVal;
	if (variableValue != 6) throw "Wrong result of INC operator.";

//	delete node;
//	delete operand;
//	delete var;
}

void MC::DriverTest::testDEC() {
	std::istringstream in;
	std::ostringstream out;
	Driver driver = MC::Driver::Builder().input(in).output(out).build();

	xVariableNode* var = variable("a", 5);
	driver.addVariable(var);

	xVariableNode* operandOne = variable("a");
	xOperatorNode* node = operatorNode(MC::Parser::token::DEC, operandOne);
	xNode* result = node->ex(&driver);

	checkResultIsConstant(result, "DEC", 4);

	xVariableNode* var2 = driver.findVariable("a");
	int variableValue = var2->value->toInt()->intVal;
	if (variableValue != 4) throw "Wrong result of INC operator.";

//	delete node;
//	delete operandOne;
//	delete var;
}
