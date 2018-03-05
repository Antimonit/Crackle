#include <string>
#include <list>
#include <map>
#include <iostream>
#include "nodes/xVariableNode.h"
#include "nodes/xConstantNode.h"
#include "nodes/xFunctionDefNode.h"
#include "nodes/xObjectDefNode.h"
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

xVariableNode* MC::SymbolTable::findVariableInTable(const std::string& symbol) {
	if (variables.find(symbol) != variables.end()) {
		return variables[symbol];
	}
	return nullptr;
}
xFunctionDefNode* MC::SymbolTable::findFunctionInTable(const std::string& symbol) {
	if (functions.find(symbol) != functions.end()) {
		return functions[symbol];
	}
	return nullptr;
}
xObjectDefNode* MC::SymbolTable::findObjectInTable(const std::string& symbol) {
	if (objects.find(symbol) != objects.end()) {
		return objects[symbol];
	}
	return nullptr;
}

xVariableNode* MC::SymbolTable::findVariable(const std::string& symbol) {
	SymbolTable* table = this;
	while (table != nullptr) {
		xVariableNode* res = table->findVariableInTable(symbol);
		if (res != nullptr) {
			return res;
		}
		table = table->parentTable;
	}
	return nullptr;
}
xFunctionDefNode* MC::SymbolTable::findFunction(const std::string& symbol) {
	SymbolTable* table = this;
	while (table != nullptr) {
		xFunctionDefNode* res = table->findFunctionInTable(symbol);
		if (res != nullptr) {
			return res;
		}
		table = table->parentTable;
	}
	return nullptr;
}
xObjectDefNode* MC::SymbolTable::findObject(const std::string& symbol) {
	SymbolTable* table = this;
	while (table != nullptr) {
		xObjectDefNode* res = table->findObjectInTable(symbol);
		if (res != nullptr) {
			return res;
		}
		table = table->parentTable;
	}
	return nullptr;
}

void MC::SymbolTable::addVariable(xVariableNode* variableDef) {
	xVariableNode* symbol = findVariableInTable(variableDef->name);
	if (symbol != nullptr) {
		std::cerr << "Warning: Trying to redeclare variable '" << variableDef->name
				  << "' of type " << symbol->value->getType()
				  << " to type " << variableDef->value->getType()
				  << "." << std::endl;
		return;
	}
	variables.insert(std::pair<std::string, xVariableNode*>(variableDef->name, variableDef));
}
void MC::SymbolTable::addFunction(xFunctionDefNode* function) {
	xFunctionDefNode* symbol = findFunctionInTable(function->name);
	if (symbol != nullptr) {
		std::cerr << "Warning: Trying to redeclare function '" << function
				  << "' as '" << *symbol
				  << "'." << std::endl;
		return;
	}
	functions.insert(std::pair<std::string, xFunctionDefNode*>(function->name, function));
}
void MC::SymbolTable::addObject(xObjectDefNode* object) {
	xObjectDefNode* symbol = findObjectInTable(object->name);
	if (symbol != nullptr) {
		std::cerr << "Warning: Trying to redeclare object '" << object->name
				  << "'." << std::endl;
		return;
	}
	objects.insert(std::pair<std::string, xObjectDefNode*>(object->name, object));
}