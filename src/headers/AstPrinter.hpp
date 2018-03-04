//
// Created by DAVE on 8. 1. 2018.
//

#ifndef __AST_PRINTER_HPP__
#define __AST_PRINTER_HPP__

#include <c++/list>
#include <c++/iostream>

class xNode;
class xOperatorNode;

namespace MC {

	class AstPrinter {
	public:

		explicit AstPrinter(std::ostream* out) : out(out ? out->rdbuf() : std::cout.rdbuf()) {
		};

		void enterNode(xNode* node);

		void exitNode(xNode* node);

	private:

		std::ostream out;
		std::list<xNode*> stack;
		int indent = 0;

		void printOperator(xOperatorNode* node);

		void printNode(bool entering, xNode* node);

	};

}

#endif //CRACKLE_INTERPRETER_AST_PRINTER_H
