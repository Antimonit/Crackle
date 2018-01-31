#ifndef __MCDRIVER_HPP__
#define __MCDRIVER_HPP__ 1

#include <string>
#include <cstddef>
#include <istream>
#include "scanner.hpp"
#include "parser.tab.hh"
#include "headers/types.hpp"
#include "headers/ast_printer.hpp"
#include "headers/symbol_table.hpp"
#include "headers/node_helpers.hpp"

namespace MC {

	class Driver {
	public:

		class Builder {
		public:
			Builder();

			std::istream* in;
			std::ostream* out;
			std::ostream* deb;

			void input(std::istream& is);

			void input(std::string filename);

			void output(std::ostream& os);

			void output(std::string filename);

			void debug(std::ostream& os);

			void debug(std::string filename);

			Driver build();

		};

		int parse();

		Node* ex(Node* p);

		virtual ~Driver();

	private:
		Driver(std::istream* in,
			   std::ostream* out,
			   std::ostream* deb);

		std::istream* in;
		std::ostream* out;
		std::ostream* deb;

		MC::Parser* parser = nullptr;
		MC::Scanner* scanner = nullptr;
		MC::AstPrinter* printer = nullptr;

		SymbolTable* rootSymbolTable = new SymbolTable();
		SymbolTable* currentSymbolTable = rootSymbolTable;

		void replaceSymbolTableScope();

		void pushSymbolTableScope();

		void popSymbolTableScope();

		VariableNode* findVariable(const std::string& symbol);

		FunctionDefNode* findFunction(const std::string& symbol);

		ObjectDefNode* findObject(const std::string& symbol);

		void addVariable(VariableNode* variableDef);

		void addFunction(FunctionDefNode& function);

		void addObject(ObjectDefNode& object);


		void returnx(Node* p, Node* result);

		void whilex(Node* p, Node* result);

		void forx(Node* p, Node* result);

		void ifx(Node* p, Node* result);

		void delimiter(Node* p, Node* result);

		void dot(Node* p, Node* result);

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

		void inc(Node* p, Node* result);

		void dec(Node* p, Node* result);

		void lt(Node* p, Node* result);

		void le(Node* p, Node* result);

		void gt(Node* p, Node* result);

		void ge(Node* p, Node* result);

		void eq(Node* p, Node* result);

		void ne(Node* p, Node* result);

		void printx(Node* p, Node* result);

		void println(Node* p, Node* result);

	};

} /* end namespace MC */

#endif /* END __MCDRIVER_HPP__ */
