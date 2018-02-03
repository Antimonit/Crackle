#ifndef __DRIVER_HPP__
#define __DRIVER_HPP__

#include <string>
#include <cstddef>
#include <istream>
#include "scanner.hpp"
#include "parser.tab.hh"
#include "headers/types.hpp"
#include "headers/AstPrinter.hpp"
#include "headers/SymbolTable.hpp"
#include "headers/node_helpers.hpp"
#include "headers/Heap.hpp"

namespace MC {

	class Driver {
	public:

		class Builder {
		private:
			std::istream* in;
			std::ostream* out;
			std::ostream* deb;
			int heapSize = DEFAULT_HEAP_SIZE;

		public:
			Builder();

			void input(std::istream& is);

			void input(std::string filename);

			void output(std::ostream& os);

			void output(std::string filename);

			void debug(std::ostream& os);

			void debug(std::string filename);

			void heap(int heapSize);

			Driver build();

		};

		int parse();

		Node* ex(Node* p);

		virtual ~Driver();

	private:
		Driver(std::istream* in,
			   std::ostream* out,
			   std::ostream* deb,
			   int heapSize
		);

		std::istream* in;
		std::ostream* out;
		std::ostream* deb;

		MC::Parser* parser = nullptr;
		MC::Scanner* scanner = nullptr;
		MC::AstPrinter* printer = nullptr;

		MC::Heap heap;

		MC::SymbolTable* rootSymbolTable = new SymbolTable();
		MC::SymbolTable* currentSymbolTable = rootSymbolTable;

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
