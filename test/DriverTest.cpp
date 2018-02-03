//
// Created by DAVE on 3. 2. 2018.
//

#include "DriverTest.hpp"

void MC::DriverTest::testEQ() {
	std::istringstream in;
	std::ostringstream out;
	Driver driver = MC::Driver::Builder().input(in).output(out).build();

	Node* operandOne = constantNode(5);
	Node* operandTwo = constantNode(5);
	Node* node = operatorNode(MC::Parser::token::EQ, operandOne, operandTwo);
	Node* result = new Node;

	driver.eq(node, result);

	if (result->getType() != Node::Type::Constant) {
		throw "Result of node EQ is not of type Constant.";
	}
	if (result->constant.getType() != typeBool) {
		throw "Result of node EQ is not of type Constant Bool.";
	}
	if (result->constant.boolVal != true) {
		throw "Result of node EQ is not correct.";
	}

	delete result;
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
	Node* result = new Node;

	driver.inc(node, result);

	if (result->getType() != Node::Type::Constant) {
		throw "Result of node INC is not of type Constant.";
	}
	if (result->constant.getType() != typeInt) {
		throw "Result of node INC is not of type Constant Bool.";
	}
	if (result->constant.intVal != 6) {
		throw "Result of node INC is not correct.";
	}

	delete result;
	delete node;
	delete operandOne;
	delete var;
}
