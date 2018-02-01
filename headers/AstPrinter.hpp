//
// Created by DAVE on 8. 1. 2018.
//

#ifndef CRACKLE_INTERPRETER_AST_PRINTER_H
#define CRACKLE_INTERPRETER_AST_PRINTER_H

#include <list>
#include "headers/types.hpp"

namespace MC {

	class AstPrinter {
	public:

		explicit AstPrinter(std::ostream* out) : out(out ? out->rdbuf() : std::cout.rdbuf()) {
		};

		void enterNode(Node* node);

		void exitNode(Node* node);

	private:

		std::ostream out;
		std::list<Node*> stack;
		int indent = 0;

		void printOperator(Node* node);

		void printNode(bool entering, Node* node);

	};

}

#endif //CRACKLE_INTERPRETER_AST_PRINTER_H
