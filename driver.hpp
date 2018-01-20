#ifndef __MCDRIVER_HPP__
#define __MCDRIVER_HPP__ 1

#include <string>
#include <cstddef>
#include <istream>

#include "scanner.hpp"
#include "parser.tab.hh"
#include <headers/types.hpp>
#include <headers/ast_printer.hpp>
#include "headers/symbol_table.hpp"
#include "headers/node_helpers.hpp"

namespace MC {

	class Driver {
	public:
		Driver() = default;

		virtual ~Driver();

		void parse(const char* filename);

		void parse(std::istream& is);

		Node* ex(Node* p);

		std::ostream &print(std::ostream &stream);

	private:

		SymbolTable* rootSymbolTable = new SymbolTable();
		SymbolTable* currentSymbolTable = rootSymbolTable;

		void replaceSymbolTableScope() {
			auto* table = new SymbolTable();
			table->parentTable = rootSymbolTable;
			table->previousTable = currentSymbolTable;
			currentSymbolTable = table;
		}

		void pushSymbolTableScope() {
			auto* table = new SymbolTable();
			table->parentTable = currentSymbolTable;
			table->previousTable = currentSymbolTable;
			currentSymbolTable = table;
		}

		void popSymbolTableScope() {
			SymbolTable* table = currentSymbolTable;
			currentSymbolTable = table->previousTable;
			delete table;
		}

		ConstantNode* findVariable(const std::string &symbol) {
			return currentSymbolTable->findVariable(symbol);
		}
		FunctionDefNode* findFunction(const std::string &symbol) {
			return currentSymbolTable->findFunction(symbol);
		}
		ObjectDefNode* findObject(const std::string &symbol) {
			return currentSymbolTable->findObject(symbol);
		}

		void addVariable(VariableDefNode* variableDef) {
			currentSymbolTable->addVariable(variableDef);
		}
		void addFunction(FunctionDefNode* function) {
			currentSymbolTable->addFunction(function);
		}
		void addObject(ObjectDefNode* object) {
			currentSymbolTable->addObject(object);
		}



		void returnx(Node* p, Node* result);
		void whilex(Node* p, Node* result);
		void forx(Node* p, Node* result);
		void ifx(Node* p, Node* result);

		void delimiter(Node* p, Node* result);
		void assign(Node* p, Node* result);
		void uminus(Node* p, Node* result);

		void plus(Node* p, Node* result);
		void minus(Node* p, Node* result);
		void multiply(Node* p, Node* result);
		void divide(Node* p, Node* result);
		void modulo(Node* p, Node* result);

		void andx(Node* p, Node* result);
		void orx(Node* p, Node* result);
		void neg(Node* p, Node* result);

		void lt(Node* p, Node* result);
		void le(Node* p, Node* result);
		void gt(Node* p, Node* result);
		void ge(Node* p, Node* result);
		void eq(Node* p, Node* result);
		void ne(Node* p, Node* result);

		void printx(Node* p, Node* result);
		void println(Node* p, Node* result);

		void parse_helper(std::istream &stream);

		std::size_t chars = 0;
		std::size_t words = 0;
		std::size_t lines = 0;
		std::size_t uppercase = 0;
		std::size_t lowercase = 0;
		MC::Parser *parser = nullptr;
		MC::Scanner *scanner = nullptr;
	};

} /* end namespace MC */

#endif /* END __MCDRIVER_HPP__ */
