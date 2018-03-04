#ifndef __SYMBOL_TABLE_HPP__
#define __SYMBOL_TABLE_HPP__

#include <cstdlib>
#include <map>

class xVariableNode;
class xFunctionDefNode;
class xObjectDefNode;

namespace MC {

	class Heap;

	class SymbolTable {
		friend class Heap;
	private:

		std::map<std::string, xVariableNode*> variables{};
		std::map<std::string, xFunctionDefNode*> functions{};
		std::map<std::string, xObjectDefNode*> objects{};

		xVariableNode* findVariableInTable(const std::string &symbol);
		xFunctionDefNode* findFunctionInTable(const std::string &symbol);
		xObjectDefNode* findObjectInTable(const std::string &symbol);

	public:

		~SymbolTable();

		SymbolTable* parentTable = nullptr;		// strictly going upwards the scope tree
		SymbolTable* previousTable = nullptr;	// can point to a sibling (when calling a function)
		// when popping the symbolTable, restore previous table and not the parent table

		xVariableNode* findVariable(const std::string& symbol);
		xFunctionDefNode* findFunction(const std::string& symbol);
		xObjectDefNode* findObject(const std::string&symbol);

		void addVariable(xVariableNode* variableDef);
		void addFunction(xFunctionDefNode* function);
		void addObject(xObjectDefNode* object);

	};

}

#endif
