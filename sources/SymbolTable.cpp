#include <string>
#include <list>
#include <map>
#include <iostream>
#include "headers/types.hpp"
#include "headers/SymbolTable.hpp"

MC::SymbolTable::~SymbolTable() {
//	for (auto variable : variables) {
//		delete variable;
//	}
//	for (auto function : functions) {
//		delete function;
//	}
//	for (auto object : objects) {
//		delete object;
//	}
}

VariableNode* MC::SymbolTable::findVariableInTable(const std::string& symbol) {
	if (variables.find(symbol) != variables.end()) {
		return variables[symbol];
	}
	return nullptr;
}
FunctionDefNode* MC::SymbolTable::findFunctionInTable(const std::string& symbol) {
	if (functions.find(symbol) != functions.end()) {
		return functions[symbol];
	}
	return nullptr;
}
ObjectDefNode* MC::SymbolTable::findObjectInTable(const std::string& symbol) {
	if (objects.find(symbol) != objects.end()) {
		return objects[symbol];
	}
	return nullptr;
}

VariableNode* MC::SymbolTable::findVariable(const std::string& symbol) {
	SymbolTable* table = this;
	while (table != nullptr) {
		VariableNode* res = table->findVariableInTable(symbol);
		if (res != nullptr) {
			return res;
		}
		table = table->parentTable;
	}
	return nullptr;
}
FunctionDefNode* MC::SymbolTable::findFunction(const std::string& symbol) {
	SymbolTable* table = this;
	while (table != nullptr) {
		FunctionDefNode* res = table->findFunctionInTable(symbol);
		if (res != nullptr) {
			return res;
		}
		table = table->parentTable;
	}
	return nullptr;
}
ObjectDefNode* MC::SymbolTable::findObject(const std::string& symbol) {
	SymbolTable* table = this;
	while (table != nullptr) {
		ObjectDefNode* res = table->findObjectInTable(symbol);
		if (res != nullptr) {
			return res;
		}
		table = table->parentTable;
	}
	return nullptr;
}

void MC::SymbolTable::addVariable(VariableNode* variableDef) {
	VariableNode* symbol = findVariableInTable(variableDef->name);
	if (symbol != nullptr) {
		std::cerr << "Warning: Trying to redeclare variable '" << variableDef->name
				  << "' of type " << symbol->value.getType()
				  << " to type " << variableDef->value.getType()
				  << "." << std::endl;
		return;
	}
	variables.insert(std::pair<std::string, VariableNode*>(variableDef->name, variableDef));
}
void MC::SymbolTable::addFunction(FunctionDefNode& function) {
	FunctionDefNode* symbol = findFunctionInTable(function.name);
	if (symbol != nullptr) {
		std::cerr << "Warning: Trying to redeclare function '" << function
				  << "' as '" << *symbol
				  << "'." << std::endl;
		return;
	}
	functions.insert(std::pair<std::string, FunctionDefNode*>(function.name, &function));
}
void MC::SymbolTable::addObject(ObjectDefNode& object) {
	ObjectDefNode* symbol = findObjectInTable(object.name);
	if (symbol != nullptr) {
		std::cerr << "Warning: Trying to redeclare object '" << object.name
				  << "'." << std::endl;
		return;
	}
	objects.insert(std::pair<std::string, ObjectDefNode*>(object.name, &object));
}