#include <c++/cctype>
#include <c++/fstream>
#include <c++/iostream>
#include "nodes/xNode.h"
#include "nodes/xEmptyNode.h"

#include "Driver.hpp"
#include "scanner.hpp"
#include "parser.tab.hh"

using token = MC::Parser::token;


MC::Driver::Builder::Builder() {
	in = &std::cin;
	out = &std::cout;
	deb = nullptr;
}

MC::Driver::Builder MC::Driver::Builder::input(std::string filename) {
	auto* file = new std::ifstream(filename);
	if (!file->good()) {
		exit(EXIT_FAILURE);
	}
	in = file;
	return *this;
}

MC::Driver::Builder MC::Driver::Builder::input(std::istream& is) {
	if (!is.good() && is.eof()) {
		return *this;
	}
	in = &is;
	return *this;
}

MC::Driver::Builder MC::Driver::Builder::output(std::string filename) {
	auto* file = new std::ofstream(filename);
	if (!file->good()) {
		exit(EXIT_FAILURE);
	}
	out = file;
	return *this;
}

MC::Driver::Builder MC::Driver::Builder::output(std::ostream& os) {
	if (!os.good() && os.eof()) {
		return *this;
	}
	out = &os;
	return *this;
}

MC::Driver::Builder MC::Driver::Builder::debug(std::string filename) {
	auto* file = new std::ofstream(filename);
	if (!file->good()) {
		exit(EXIT_FAILURE);
	}
	deb = file;
	return *this;
}

MC::Driver::Builder MC::Driver::Builder::debug(std::ostream& os) {
	if (!os.good() && os.eof()) {
		return *this;
	}
	deb = &os;
	return *this;
}

MC::Driver::Builder MC::Driver::Builder::heap(int heapSize) {
	this->heapSize = heapSize;
	return *this;
}

MC::Driver MC::Driver::Builder::build() {
	return Driver(in, out, deb, heapSize);
};


MC::Driver::Driver(std::istream* in,
				   std::ostream* out,
				   std::ostream* deb,
				   int heapSize): in(in), out(out), deb(deb), heap(heapSize) {
}

MC::Driver::~Driver() {
	delete scanner;
	scanner = nullptr;
	delete parser;
	parser = nullptr;
	delete printer;
	printer = nullptr;
}

int MC::Driver::parse() {
	if (!in->good() && in->eof()) {
		return 1;
	}

	delete scanner;
	try {
		scanner = new MC::Scanner(in);
	} catch (std::bad_alloc& ba) {
		std::cerr << "Failed to allocate scanner: (" << ba.what() << "), exiting!!\n";
		exit(EXIT_FAILURE);
	}

	delete parser;
	try {
		parser = new MC::Parser((*scanner) /* scanner */,
								(*this) /* driver */ );
	} catch (std::bad_alloc& ba) {
		std::cerr << "Failed to allocate parser: (" << ba.what() << "), exiting!!\n";
		exit(EXIT_FAILURE);
	}

	delete printer;
	if (deb != nullptr) {
		try {
			printer = new MC::AstPrinter(deb);
		} catch (std::bad_alloc& ba) {
			std::cerr << "Failed to allocate printer: (" << ba.what() << "), exiting!!\n";
			exit(EXIT_FAILURE);
		}
	} else {
		printer = nullptr;
	}

	return parser->parse();
}


xNode* MC::Driver::ex(xNode* p) {
	if (!p) {
		return new xEmptyNode();
	}

	if (printer != nullptr) {
		printer->enterNode(p);
	}

	xNode* result = p->ex(this);

	if (printer != nullptr) {
		printer->exitNode(result);
	}

	return result;
}


void MC::Driver::replaceSymbolTableScope() {
	auto* table = new SymbolTable();
	table->parentTable = rootSymbolTable;
	table->previousTable = currentSymbolTable;
	currentSymbolTable = table;
}

void MC::Driver::pushSymbolTableScope() {
	auto* table = new SymbolTable();
	table->parentTable = currentSymbolTable;
	table->previousTable = currentSymbolTable;
	currentSymbolTable = table;
}

void MC::Driver::popSymbolTableScope() {
	SymbolTable* table = currentSymbolTable;
	currentSymbolTable = table->previousTable;
	delete table;
}

xVariableNode* MC::Driver::findVariable(const std::string& symbol) {
	return currentSymbolTable->findVariable(symbol);
}

xFunctionDefNode* MC::Driver::findFunction(const std::string& symbol) {
	return currentSymbolTable->findFunction(symbol);
}

xObjectDefNode* MC::Driver::findObject(const std::string& symbol) {
	return currentSymbolTable->findObject(symbol);
}

void MC::Driver::addVariable(xVariableNode* variableDef) {
	currentSymbolTable->addVariable(variableDef);
}

void MC::Driver::addFunction(xFunctionDefNode* function) {
	currentSymbolTable->addFunction(function);
}

void MC::Driver::addObject(xObjectDefNode* object) {
	currentSymbolTable->addObject(object);
}


