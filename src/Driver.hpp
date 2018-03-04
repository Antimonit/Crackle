#ifndef __DRIVER_HPP__
#define __DRIVER_HPP__

#include <c++/string>
#include <c++/cstddef>
#include <c++/istream>
//#include "scanner.hpp"
#include "parser.tab.hh"

#include "headers/AstPrinter.hpp"
#include "headers/SymbolTable.hpp"
#include "headers/node_helpers.hpp"
#include "headers/Heap.hpp"

class xNode;

namespace MC {

	class DriverTest;
//	class Scanner;
//	class Parser;
	class Driver {

//#ifdef UNIT_TEST
		friend DriverTest;
//#endif

	public:

		class Builder {
		private:
			std::istream* in;
			std::ostream* out;
			std::ostream* deb;
			int heapSize = DEFAULT_HEAP_SIZE;

		public:
			Builder();

			MC::Driver::Builder input(std::istream& is);

			MC::Driver::Builder input(std::string filename);

			MC::Driver::Builder output(std::ostream& os);

			MC::Driver::Builder output(std::string filename);

			MC::Driver::Builder debug(std::ostream& os);

			MC::Driver::Builder debug(std::string filename);

			MC::Driver::Builder heap(int heapSize);

			Driver build();

		};

		int parse();

		xNode* ex(xNode* p);

		virtual ~Driver();

		void replaceSymbolTableScope();

		void pushSymbolTableScope();

		void popSymbolTableScope();

		xVariableNode* findVariable(const std::string& symbol);

		xFunctionDefNode* findFunction(const std::string& symbol);

		xObjectDefNode* findObject(const std::string& symbol);

		void addVariable(xVariableNode* variableDef);

		void addFunction(xFunctionDefNode* function);

		void addObject(xObjectDefNode* object);

		xObjectNode* allocateNewObject() {
			if (heap.isFull()) {
				heap.performGarbageCollection(currentSymbolTable);
			}
			return heap.allocateNewObject();
		}

		std::istream* in;
		std::ostream* out;
		std::ostream* deb;

	private:
		Driver(std::istream* in,
			   std::ostream* out,
			   std::ostream* deb,
			   int heapSize
		);

		MC::Parser* parser = nullptr;
		MC::Scanner* scanner = nullptr;
		MC::AstPrinter* printer = nullptr;

		MC::Heap heap;

		MC::SymbolTable* rootSymbolTable = new SymbolTable();
		MC::SymbolTable* currentSymbolTable = rootSymbolTable;

	};

} /* end namespace MC */

#endif /* END __MCDRIVER_HPP__ */
