#include <stdio.h>
#include <string>
#include <list>
#include <map>
#include <iostream>
#include "headers/types.hpp"
#include "headers/node_helpers.hpp"
#include "headers/symbol_table.hpp"


class symbolTable {
private:

	std::map<std::string, ConstantNode*> variables;
	std::map<std::string, FunctionDefNode*> functions;
	std::map<std::string, ObjectDefNode*> objects;

	struct symbolTable* parentTable = nullptr;		// strictly going upwards the scope tree
	struct symbolTable* previousTable = nullptr;	// can point to a sibling (when calling a function)
	// when popping the symbolTable, restore previous table and not the parent table

	ConstantNode* findVariableInTable(const std::string &symbol);
	FunctionDefNode* findFunctionInTable(const std::string &symbol);
	ObjectDefNode* findObjectInTable(const std::string &symbol);

public:

	static void replaceSymbolTableScope();
	static void pushSymbolTableScope();
	static void popSymbolTableScope();

	static ConstantNode* findVariable(const std::string &symbol);
	static void addVariable(VariableDefNode* variableDef);

	static FunctionDefNode* findFunction(const std::string &symbol);
	static void addFunction(FunctionDefNode* function);

	static ObjectDefNode* findObject(const std::string &symbol);
	static void addObject(ObjectDefNode* object);
};

symbolTable* rootSymbolTable = new symbolTable();
symbolTable* currentSymbolTable = rootSymbolTable;


ConstantNode* symbolTable::findVariableInTable(const std::string &symbol) {
	if (variables.find(symbol) != variables.end()) {
		return variables[symbol];
	}
	return nullptr;
}

FunctionDefNode* symbolTable::findFunctionInTable(const std::string &symbol) {
	if (functions.find(symbol) != functions.end()) {
		return functions[symbol];
	}
	return nullptr;
}

ObjectDefNode* symbolTable::findObjectInTable(const std::string &symbol) {
	if (objects.find(symbol) != objects.end()) {
		return objects[symbol];
	}
	return nullptr;
}


void symbolTable::replaceSymbolTableScope() {
	auto * table = new symbolTable();
	table->parentTable = rootSymbolTable;
	table->previousTable = currentSymbolTable;
	currentSymbolTable = table;
}

void symbolTable::pushSymbolTableScope() {
	auto * table = new symbolTable();
	table->parentTable = currentSymbolTable;
	table->previousTable = currentSymbolTable;
	currentSymbolTable = table;
}

void symbolTable::popSymbolTableScope() {
	symbolTable* table = currentSymbolTable;
	currentSymbolTable = table->previousTable;
	delete table;
}


ConstantNode* symbolTable::findVariable(const std::string &symbol) {
	symbolTable* table = currentSymbolTable;

	while (table != nullptr) {
		ConstantNode *res = table->findVariableInTable(symbol);
		if (res != nullptr) {
			return res;
		}
		table = table->parentTable;
	}
	return nullptr;
}

void symbolTable::addVariable(VariableDefNode* variableDef) {
	symbolTable *table = currentSymbolTable;
	ConstantNode* symbolNode = table->findVariableInTable(variableDef->name);
	if (symbolNode != nullptr) {
		std::cerr << "Warning: Trying to redeclare variable '" << variableDef->name
				  << "' of type " << dataTypeToString(symbolNode->dataType)
				  << " to type " << dataTypeToString(variableDef->value.dataType)
				  << "." << std::endl;
		return;
	}

	// new symbol
	table->variables.insert(std::pair<std::string, ConstantNode *>(std::string(variableDef->name), &variableDef->value));
}


FunctionDefNode* symbolTable::findFunction(const std::string &symbol) {
	symbolTable *table = currentSymbolTable;

	while (table != nullptr) {
		FunctionDefNode *res = table->findFunctionInTable(symbol);
		if (res != nullptr) {
			return res;
		}
		table = table->parentTable;
	}
	return nullptr;
}

void symbolTable::addFunction(FunctionDefNode* function) {
	symbolTable *table = currentSymbolTable;
	FunctionDefNode *symbol = table->findFunctionInTable(function->name);
	if (symbol != nullptr) {
		std::cerr << "Warning: Trying to redeclare function '" << function->name
				  << "'." << std::endl;
		return;
	}
	// new symbol
	table->functions.insert(std::pair<std::string, FunctionDefNode*>(std::string(function->name), function));
}


ObjectDefNode* symbolTable::findObject(const std::string &symbol) {
	symbolTable *table = currentSymbolTable;

	while (table != nullptr) {
		ObjectDefNode *res = table->findObjectInTable(symbol);
		if (res != nullptr) {
			return res;
		}
		table = table->parentTable;
	}
	return nullptr;
}

void symbolTable::addObject(ObjectDefNode* object) {
	symbolTable *table = currentSymbolTable;
	ObjectDefNode *symbol = table->findObjectInTable(object->name);
	if (symbol != nullptr) {
		std::cerr << "Warning: Trying to redeclare object '" << object->name
				  << "'." << std::endl;
		return;
	}
	// new symbol
	table->objects.insert(std::pair<std::string, ObjectDefNode*>(std::string(object->name), object));
}


void replaceSymbolTableScope() {
	symbolTable::replaceSymbolTableScope();
}

void pushSymbolTableScope() {
	symbolTable::pushSymbolTableScope();
}

void popSymbolTableScope() {
	symbolTable::popSymbolTableScope();
}


ConstantNode* findVariable(const std::string &symbol) {
	return symbolTable::findVariable(symbol);
}

void addVariable(VariableDefNode* variable) {
	symbolTable::addVariable(variable);
}


FunctionDefNode* findFunction(const std::string &symbol) {
	return symbolTable::findFunction(symbol);
}

void addFunction(FunctionDefNode* function) {
	symbolTable::addFunction(function);
}


ObjectDefNode* findObject(const std::string &symbol) {
	return symbolTable::findObject(symbol);
}

void addObject(ObjectDefNode* object) {
	symbolTable::addObject(object);
}