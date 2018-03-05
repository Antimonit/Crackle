//
// Created by DAVE on 3. 2. 2018.
//

#ifndef __DRIVER_TEST_HPP__
#define __DRIVER_TEST_HPP__

#include <c++/sstream>
#include "Driver.hpp"
#include "nodes/constant/xConstantIntNode.h"
#include "nodes/constant/xConstantDoubleNode.h"
#include "nodes/constant/xConstantBoolNode.h"
#include "nodes/constant/xConstantStringNode.h"
#include "nodes/constant/xConstantVoidNode.h"
#include "nodes/constant/xConstantObjectNode.h"

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

		static xVariableNode* variable(const std::string& name) {
			return new xVariableNode(name, nullptr);
		}
		static xVariableNode* variable(const std::string& name, int val) {
			return new xVariableNode(name, new xConstantIntNode(val));
		}
		static xVariableNode* variable(const std::string& name, double val) {
			return new xVariableNode(name, new xConstantDoubleNode(val));
		}
		static xVariableNode* variable(const std::string& name, std::string val) {
			return new xVariableNode(name, new xConstantStringNode(val));
		}
		static xVariableNode* variable(const std::string& name, bool val) {
			return new xVariableNode(name, new xConstantBoolNode(val));
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
