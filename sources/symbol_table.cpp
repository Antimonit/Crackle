#include <string>
#include <list>
#include <map>
#include <iostream>
#include "headers/types.hpp"
#include "headers/symbol_table.hpp"

ConstantNode* SymbolTable::findVariableInTable(const std::string &symbol) {
	if (variables.find(symbol) != variables.end()) {
		return variables[symbol];
	}
	return nullptr;
}
FunctionDefNode* SymbolTable::findFunctionInTable(const std::string &symbol) {
	if (functions.find(symbol) != functions.end()) {
		return functions[symbol];
	}
	return nullptr;
}
ObjectDefNode* SymbolTable::findObjectInTable(const std::string &symbol) {
	if (objects.find(symbol) != objects.end()) {
		return objects[symbol];
	}
	return nullptr;
}

ConstantNode* SymbolTable::findVariable(const std::string &symbol) {
	SymbolTable* table = this;
	while (table != nullptr) {
		ConstantNode *res = table->findVariableInTable(symbol);
		if (res != nullptr) {
			return res;
		}
		table = table->parentTable;
	}
	return nullptr;
}
FunctionDefNode* SymbolTable::findFunction(const std::string &symbol) {
	SymbolTable* table = this;
	while (table != nullptr) {
		FunctionDefNode *res = table->findFunctionInTable(symbol);
		if (res != nullptr) {
			return res;
		}
		table = table->parentTable;
	}
	return nullptr;
}
ObjectDefNode* SymbolTable::findObject(const std::string &symbol) {
	SymbolTable* table = this;
	while (table != nullptr) {
		ObjectDefNode *res = table->findObjectInTable(symbol);
		if (res != nullptr) {
			return res;
		}
		table = table->parentTable;
	}
	return nullptr;
}

void SymbolTable::addVariable(VariableDefNode* variableDef) {
	ConstantNode* symbolNode = findVariableInTable(variableDef->name);
	if (symbolNode != nullptr) {
		std::cerr << "Warning: Trying to redeclare variable '" << variableDef->name
				  << "' of type " << symbolNode->getType()
				  << " to type " << variableDef->value.getType()
				  << "." << std::endl;
		return;
	}
	variables.insert(std::pair<std::string, ConstantNode*>(variableDef->name, &variableDef->value));
}
void SymbolTable::addFunction(FunctionDefNode* function) {
	FunctionDefNode *symbol = findFunctionInTable(function->name);
	if (symbol != nullptr) {
		std::cerr << "Warning: Trying to redeclare function '" << function->name
				  << "'." << std::endl;
		return;
	}
	functions.insert(std::pair<std::string, FunctionDefNode*>(function->name, function));
}
void SymbolTable::addObject(ObjectDefNode* object) {
	ObjectDefNode *symbol = findObjectInTable(object->name);
	if (symbol != nullptr) {
		std::cerr << "Warning: Trying to redeclare object '" << object->name
				  << "'." << std::endl;
		return;
	}
	objects.insert(std::pair<std::string, ObjectDefNode*>(object->name, object));
}