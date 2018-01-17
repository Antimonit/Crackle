#include <stdio.h>
#include <string>
#include <list>
#include <map>
#include <headers/types.hpp>
#include <headers/node_helpers.hpp>
#include "headers/symbol_table.hpp"


class symbolTable {
private:

	std::map<std::string, constantNode*> variables;
	std::map<std::string, functionDefNode*> functions;
	std::map<std::string, objectDefNode*> objects;

	struct symbolTable* parentTable;	// strictly going upwards the scope tree
	struct symbolTable* previousTable;	// can point to a sibling (when calling a function)
	// when popping the symbolTable, restore previous table and not the parent table

	constantNode* findVariableInTable(const char *symbol);
	functionDefNode* findFunctionInTable(const char *symbol);
	objectDefNode* findObjectInTable(const char *symbol);

public:

	static void replaceSymbolTableScope();
	static void pushSymbolTableScope();
	static void popSymbolTableScope();

	static constantNode* findVariable(const char *symbol);
	static void addVariable(variableDefNode* variableDef);

	static functionDefNode* findFunction(const char *symbol);
	static void addFunction(functionDefNode* function);

	static objectDefNode* findObject(const char* symbol);
	static void addObject(objectDefNode* object);
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

functionDefNode* symbolTable::findFunctionInTable(const char *symbol) {
	std::string symbolString(symbol);
	if (functions.find(symbolString) != functions.end()) {
		return functions[symbolString];
	}
	return nullptr;
}

objectDefNode* symbolTable::findObjectInTable(const char *symbol) {
	std::string symbolString(symbol);
	if (objects.find(symbolString) != objects.end()) {
		return objects[symbolString];
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

void symbolTable::addVariable(variableDefNode* variableDef) {
	symbolTable *table = currentSymbolTable;
	constantNode* symbolNode = table->findVariableInTable(variableDef->name);
	if (symbolNode != nullptr) {
		printf("Warning: Trying to redeclare variable '%s' of type %s to type %s.\n",
			   variableDef->name,
			   dataTypeToString(symbolNode->dataType),
			   dataTypeToString(variableDef->dataType));
		return;
	}

	// new symbol
	constantNode* variable = new constantNode();
	copyVariableDefToConstant(variable, variableDef);

	table->variables.insert(std::pair<std::string, constantNode *>(std::string(variableDef->name), variable));
}


functionDefNode* symbolTable::findFunction(const char *symbol) {
	symbolTable *table = currentSymbolTable;

	while (table != nullptr) {
		functionDefNode *res = table->findFunctionInTable(symbol);
		if (res != nullptr) {
			return res;
		}
		table = table->parentTable;
	}
	return nullptr;
}

void symbolTable::addFunction(functionDefNode* function) {
	symbolTable *table = currentSymbolTable;
	functionDefNode *symbol = table->findFunctionInTable(function->name);
	if (symbol != nullptr) {
		printf("Warning: Trying to redeclare function '%s'\n", function->name);
		return;
	}
	// new symbol
	table->functions.insert(std::pair<std::string, functionDefNode*>(std::string(function->name), function));
}


objectDefNode* symbolTable::findObject(const char* symbol) {
	symbolTable *table = currentSymbolTable;

	while (table != nullptr) {
		objectDefNode *res = table->findObjectInTable(symbol);
		if (res != nullptr) {
			return res;
		}
		table = table->parentTable;
	}
	return nullptr;
}

void symbolTable::addObject(objectDefNode* object) {
	symbolTable *table = currentSymbolTable;
	objectDefNode *symbol = table->findObjectInTable(object->name);
	if (symbol != nullptr) {
		printf("Warning: Trying to redeclare object '%s'\n", object->name);
		return;
	}
	// new symbol
	table->objects.insert(std::pair<std::string, objectDefNode*>(std::string(object->name), object));
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


extern "C" constantNode* findVariable(const char* symbol) {
	return symbolTable::findVariable(symbol);
}

extern "C" void addVariable(variableDefNode* variable) {
	symbolTable::addVariable(variable);
}


extern "C" functionDefNode* findFunction(const char* symbol) {
	return symbolTable::findFunction(symbol);
}

extern "C" void addFunction(functionDefNode* function) {
	symbolTable::addFunction(function);
}


extern "C" objectDefNode* findObject(const char* symbol) {
	return symbolTable::findObject(symbol);
}

extern "C" void addObject(objectDefNode* object) {
	symbolTable::addObject(object);
}