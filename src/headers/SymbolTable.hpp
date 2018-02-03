#ifndef __SYMBOL_TABLE_HPP__
#define __SYMBOL_TABLE_HPP__

#include <cstdlib>
#include <map>
#include "types.hpp"

namespace MC {

	class Heap;

	class SymbolTable {
		friend class Heap;
	private:

		std::map<std::string, VariableNode*> variables{};
		std::map<std::string, FunctionDefNode*> functions{};
		std::map<std::string, ObjectDefNode*> objects{};

		VariableNode* findVariableInTable(const std::string &symbol);
		FunctionDefNode* findFunctionInTable(const std::string &symbol);
		ObjectDefNode* findObjectInTable(const std::string &symbol);

	public:

		~SymbolTable();

		SymbolTable* parentTable = nullptr;		// strictly going upwards the scope tree
		SymbolTable* previousTable = nullptr;	// can point to a sibling (when calling a function)
		// when popping the symbolTable, restore previous table and not the parent table

		VariableNode* findVariable(const std::string& symbol);
		FunctionDefNode* findFunction(const std::string& symbol);
		ObjectDefNode* findObject(const std::string&symbol);

		void addVariable(VariableNode* variableDef);
		void addFunction(FunctionDefNode& function);
		void addObject(ObjectDefNode& object);

	};

}

#endif
