//
// Created by DAVE on 3. 2. 2018.
//

#include "DriverTest.hpp"

void checkResultIsConstant(Node& result, const std::string& name, int value) {
	if (result.getType() != Node::Type::Constant) throw "Result of node " + name + " is not of type Constant.";
	if (result.constant.getType() != typeInt) throw "Result of node " + name + " is not of type Constant " + typeInt + ".";
	if (result.constant.intVal != value) throw "Result of node " + name + " is not correct.";
}
void checkResultIsConstant(Node& result, const std::string& name, bool value) {
	if (result.getType() != Node::Type::Constant) throw "Result of node " + name + " is not of type Constant.";
	if (result.constant.getType() != typeBool) throw "Result of node " + name + " is not of type Constant " + typeBool + ".";
	if (result.constant.boolVal != value) throw "Result of node " + name + " is not correct.";
}
void checkResultIsConstant(Node& result, const std::string& name, double value) {
	if (result.getType() != Node::Type::Constant) throw "Result of node " + name + " is not of type Constant.";
	if (result.constant.getType() != typeDouble) throw "Result of node " + name + " is not of type Constant " + typeDouble + ".";
	if (result.constant.doubleVal != value) throw "Result of node " + name + " is not correct.";
}
void checkResultIsConstant(Node& result, const std::string& name, const std::string& value) {
	if (result.getType() != Node::Type::Constant) throw "Result of node " + name + " is not of type Constant.";
	if (result.constant.getType() != typeString) throw "Result of node " + name + " is not of type Constant " + typeString + ".";
	if (result.constant.stringVal != value) throw "Result of node " + name + " is not correct.";
}
void checkResultIsConstantUndefined(Node& result, const std::string& name) {
	if (result.getType() != Node::Type::Constant) throw "Result of node " + name + " is not of type Constant.";
	if (result.constant.getType() != typeUndefined) throw "Result of node " + name + " is not of type Constant " + typeUndefined + ".";
}

Node MC::DriverTest::testPLUS(Node* operandOne, Node* operandTwo) {
	std::istringstream in;
	std::ostringstream out;
	Driver driver = MC::Driver::Builder().input(in).output(out).build();

	Node* node = operatorNode('+', operandOne, operandTwo);
	Node result;

	driver.plus(node, &result);

	delete node;
	delete operandTwo;
	delete operandOne;

	return result;
}
void MC::DriverTest::testPLUSIntInt() {
	Node* operandOne = constantNode(5);
	Node* operandTwo = constantNode(5);
	Node result = testPLUS(operandOne, operandTwo);

	checkResultIsConstant(result, "PLUS", 10);
}
void MC::DriverTest::testPLUSDoubleDouble() {
	Node* operandOne = constantNode(5.1);
	Node* operandTwo = constantNode(5.2);
	Node result = testPLUS(operandOne, operandTwo);

	checkResultIsConstant(result, "PLUS", 10.3);
}
void MC::DriverTest::testPLUSIntDouble() {
	Node* operandOne = constantNode(5);
	Node* operandTwo = constantNode(5.0);
	Node result = testPLUS(operandOne, operandTwo);

	checkResultIsConstant(result, "PLUS", 10.0);
}
void MC::DriverTest::testPLUSIntBoolean() {
	Node* operandOne = constantNode(5);
	Node* operandTwo = constantNode(false);
	Node result = testPLUS(operandOne, operandTwo);

	checkResultIsConstantUndefined(result, "PLUS");
}

void MC::DriverTest::testEQ() {
	std::istringstream in;
	std::ostringstream out;
	Driver driver = MC::Driver::Builder().input(in).output(out).build();

	Node* operandOne = constantNode(5);
	Node* operandTwo = constantNode(5);
	Node* node = operatorNode(MC::Parser::token::EQ, operandOne, operandTwo);
	Node result;

	driver.eq(node, &result);

	checkResultIsConstant(result, "EQ", true);

	delete node;
	delete operandTwo;
	delete operandOne;
}

void MC::DriverTest::testINC() {
	std::istringstream in;
	std::ostringstream out;
	Driver driver = MC::Driver::Builder().input(in).output(out).build();

	VariableNode* var = variable("a", 5);
	driver.addVariable(var);

	Node* operandOne = variableNode("a");
	Node* node = operatorNode(MC::Parser::token::INC, operandOne);
	Node result;

	driver.inc(node, &result);

	checkResultIsConstant(result, "INC", 6);

	delete node;
	delete operandOne;
	delete var;
}
