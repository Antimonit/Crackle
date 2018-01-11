#include <stdio.h>
#include <string>
#include <list>
#include <map>
#include <headers/types.hpp>
#include "headers/symbol_table.hpp"


class symbolTable {
private:

	std::map<std::string, constantNode*> variables;
	std::map<std::string, functionNode*> functions;

	struct symbolTable* parentTable;	// strictly going upwards the scope tree
	struct symbolTable* previousTable;	// can point to a sibling (when calling a function)
	// when popping the symbolTable, restore previous table and not the parent table

	constantNode* findVariableInTable(const char *symbol);
	functionNode* findFunctionInTable(const char *symbol);

public:

	static void replaceSymbolTableScope();
	static void pushSymbolTableScope();
	static void popSymbolTableScope();

	static constantNode* findVariable(const char *symbol);
	static constantNode* addVariable(variableNode variableNode);

	static functionNode* findFunction(const char *symbol);
	static void addFunction(functionNode* function);

};

symbolTable* rootSymbolTable = new symbolTable();
symbolTable* currentSymbolTable = rootSymbolTable;


constantNode* symbolTable::findVariableInTable(const char *symbol) {
	std::string symbolString(symbol);
	if (variables.find(symbolString) != variables.end()) {
		return variables[symbolString];
	}
	return nullptr;
}

functionNode* symbolTable::findFunctionInTable(const char *symbol) {
	std::string symbolString(symbol);
	if (functions.find(symbolString) != functions.end()) {
		return functions[symbolString];
	}
	return nullptr;
}

void symbolTable::replaceSymbolTableScope() {
	symbolTable* table = new symbolTable();
	table->parentTable = rootSymbolTable;
	table->previousTable = currentSymbolTable;
	currentSymbolTable = table;
}

void symbolTable::pushSymbolTableScope() {
	symbolTable* table = new symbolTable();
	table->parentTable = currentSymbolTable;
	table->previousTable = currentSymbolTable;
	currentSymbolTable = table;
}

void symbolTable::popSymbolTableScope() {
	symbolTable* table = currentSymbolTable;
	currentSymbolTable = table->previousTable;
	delete table;
}


constantNode* symbolTable::findVariable(const char *symbol) {
	symbolTable* table = currentSymbolTable;

	while (table != nullptr) {
		constantNode *res = table->findVariableInTable(symbol);
		if (res != nullptr) {
			return res;
		}
		table = table->parentTable;
	}
	return nullptr;
}

constantNode* symbolTable::addVariable(variableNode variableNode) {
	symbolTable *table = currentSymbolTable;

	const char* name = variableNode.name;
	dataTypeEnum dataType = variableNode.dataType;

	constantNode* symbolNode = table->findVariableInTable(name);
	if (symbolNode == nullptr) {	// new symbol
		auto *variable = new constantNode();
		variable->dataType = dataType;
		table->variables.insert(std::pair<std::string, constantNode*>(std::string(name), variable));
		return variable;
	} else {			// existing symbol
		printf("Warning: Trying to redeclare variable '%s' of type %s to type %s.\n",
			   name,
			   dataTypeToString(symbolNode->dataType),
			   dataTypeToString(dataType));
		return nullptr;
	}
}


functionNode* symbolTable::findFunction(const char *symbol) {
	symbolTable *table = currentSymbolTable;

	while (table != nullptr) {
		functionNode *res = table->findFunctionInTable(symbol);
		if (res != nullptr) {
			return res;
		}
		table = table->parentTable;
	}
	return nullptr;
}

void symbolTable::addFunction(functionNode* function) {
	symbolTable *table = currentSymbolTable;
	if (findFunction(function->name) != nullptr) {
		printf("Warning: Trying to redeclare function '%s'\n", function->name);
		return;
	}
	// new symbol
	table->functions.insert(std::pair<std::string, functionNode*>(std::string(function->name), function));
}




extern "C" void replaceSymbolTableScope() {
	symbolTable::replaceSymbolTableScope();
}

extern "C" void pushSymbolTableScope() {
	symbolTable::pushSymbolTableScope();
}

extern "C" void popSymbolTableScope() {
	symbolTable::popSymbolTableScope();
}


extern "C" constantNode* findVariable(const char *symbol) {
	return symbolTable::findVariable(symbol);
}

extern "C" constantNode* addVariable(variableNode variableNode) {
	return symbolTable::addVariable(variableNode);
}


extern "C" functionNode* findFunction(const char *symbol) {
	return symbolTable::findFunction(symbol);
}

extern "C" void addFunction(functionNode* function) {
	symbolTable::addFunction(function);
}


