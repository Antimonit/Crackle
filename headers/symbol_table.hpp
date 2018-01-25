#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <cstdlib>
#include <map>
#include "types.hpp"

class SymbolTable {
private:

	std::map<std::string, VariableNode*> variables{};
	std::map<std::string, FunctionDefNode*> functions{};
	std::map<std::string, ObjectDefNode*> objects{};

	VariableNode* findVariableInTable(const std::string &symbol);
	FunctionDefNode* findFunctionInTable(const std::string &symbol);
	ObjectDefNode* findObjectInTable(const std::string &symbol);

public:

	~SymbolTable();

	struct SymbolTable* parentTable = nullptr;		// strictly going upwards the scope tree
	struct SymbolTable* previousTable = nullptr;	// can point to a sibling (when calling a function)
	// when popping the symbolTable, restore previous table and not the parent table

	VariableNode* findVariable(const std::string& symbol);
	FunctionDefNode* findFunction(const std::string& symbol);
	ObjectDefNode* findObject(const std::string&symbol);

	void addVariable(VariableNode* variableDef);
	void addFunction(FunctionDefNode& function);
	void addObject(ObjectDefNode& object);
};

#endif
