//
// Created by DAVE on 3. 2. 2018.
//

#ifndef __DRIVER_TEST_HPP__
#define __DRIVER_TEST_HPP__

#include <c++/sstream>
#include "Driver.hpp"

/*

Rather than the nasty #define hack you mention in the question,
a cleaner mechanism is to make the test a friend of the class
under test. This allows the test code (and just the test code)
access to the privates, whilst protecting them from everything
else.

However, it is preferable to test through the public interface.
If your class X has a lot of code in the private member functions
then it might be worth extracting a new class Y which is used by
the implementation of class X. This new class Y can then be tested
through its public interface, without exposing its use to the
clients of class X.

 */

namespace MC {

	class DriverTest {
	private:
		static Node* constantNode(int val) {
			Node* node = new Node;
			node->setType(Node::Type::Constant);
			node->constant.setType(typeInt);
			node->constant.intVal = val;
			return node;
		}
		static Node* constantNode(double val) {
			auto* node = new Node;
			node->setType(Node::Type::Constant);
			node->constant.setType(typeDouble);
			node->constant.doubleVal = val;
			return node;
		}
		static Node* constantNode(std::string val) {
			auto* node = new Node;
			node->setType(Node::Type::Constant);
			node->constant.setType(typeString);
			node->constant.stringVal = val;
			return node;
		}
		static Node* constantNode(bool val) {
			auto* node = new Node;
			node->setType(Node::Type::Constant);
			node->constant.setType(typeBool);
			node->constant.boolVal = val;
			return node;
		}

		static VariableNode* variable(const std::string& name) {
			auto var = new VariableNode;
			var->name = name;
			return var;
		}
		static VariableNode* variable(const std::string& name, int val) {
			auto var = variable(name);
			var->value.setType(typeInt);
			var->value.intVal = val;
			return var;
		}
		static VariableNode* variable(const std::string& name, double val) {
			auto var = variable(name);
			var->value.setType(typeDouble);
			var->value.doubleVal = val;
			return var;
		}
		static VariableNode* variable(const std::string& name, std::string val) {
			auto var = variable(name);
			var->value.setType(typeString);
			var->value.stringVal = val;
			return var;
		}
		static VariableNode* variable(const std::string& name, bool val) {
			auto var = variable(name);
			var->value.setType(typeBool);
			var->value.boolVal = val;
			return var;
		}

		static Node* variableNode(const std::string& name) {
			auto* node = new Node;
			node->setType(Node::Type::Variable);
			node->variable = variable(name);
			return node;
		}

		static Node* operatorNode(int oper) {
			Node* node = new Node;
			node->setType(Node::Type::Operator);
			node->oper.oper = oper;
			return node;
		}
		static Node* operatorNode(int oper, Node* operandOne) {
			Node* node = new Node;
			node->setType(Node::Type::Operator);
			node->oper.oper = oper;
			node->oper.op.push_back(operandOne);
			return node;
		}
		static Node* operatorNode(int oper, Node* operandOne, Node* operandTwo) {
			Node* node = new Node;
			node->setType(Node::Type::Operator);
			node->oper.oper = oper;
			node->oper.op.push_back(operandOne);
			node->oper.op.push_back(operandTwo);
			return node;
		}

		static Node testPLUS(Node* operandOne, Node* operandTwo);

	public:

		static void testPLUSIntInt();
		static void testPLUSDoubleDouble();
		static void testPLUSIntDouble();
		static void testPLUSIntBoolean();

		static void testEQ();

		static void testINC();

	};

}

#endif
