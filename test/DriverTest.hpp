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
		static xConstantNode* constantNode(int val) {
			return new xConstantNode(val);
		}
		static xConstantNode* constantNode(double val) {
			return new xConstantNode(val);
		}
		static xConstantNode* constantNode(std::string val) {
			return new xConstantNode(val);
		}
		static xConstantNode* constantNode(bool val) {
			return new xConstantNode(val);
		}

		static xVariableNode* variable(const std::string& name) {
			auto var = new xVariableNode;
			var->name = name;
			return var;
		}
		static xVariableNode* variable(const std::string& name, int val) {
			auto var = variable(name);
			var->value->setType(typeInt);
			var->value->intVal = val;
			return var;
		}
		static xVariableNode* variable(const std::string& name, double val) {
			auto var = variable(name);
			var->value->setType(typeDouble);
			var->value->doubleVal = val;
			return var;
		}
		static xVariableNode* variable(const std::string& name, std::string val) {
			auto var = variable(name);
			var->value->setType(typeString);
			var->value->stringVal = val;
			return var;
		}
		static xVariableNode* variable(const std::string& name, bool val) {
			auto var = variable(name);
			var->value->setType(typeBool);
			var->value->boolVal = val;
			return var;
		}

		static xOperatorNode* operatorNode(int oper) {
			return new xOperatorNode(oper);
		}
		static xOperatorNode* operatorNode(int oper, xNode* operandOne) {
			auto* node = new xOperatorNode(oper);
			node->op.push_back(operandOne);
			return node;
		}
		static xOperatorNode* operatorNode(int oper, xNode* operandOne, xNode* operandTwo) {
			auto* node = new xOperatorNode(oper);
			node->op.push_back(operandOne);
			node->op.push_back(operandTwo);
			return node;
		}

		static xNode* testPLUS(xConstantNode* operandOne, xConstantNode* operandTwo);

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
