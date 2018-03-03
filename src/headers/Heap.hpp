#ifndef __HEAP_HPP__
#define __HEAP_HPP__

#define DEFAULT_HEAP_SIZE 1024

#include "headers/types.hpp"
#include "SymbolTable.hpp"

namespace MC {

	class Heap {
	private:
		int size;
		int allocatedObjects;
		xObjectNode** objects;

		int findFreeSlotInHeap();;

		void gcMarkAllTables(SymbolTable* table);

		void gcMarkTable(SymbolTable* table);

		void gcMarkNode(xObjectNode* object);

		void gcSweep();

	public:

		explicit Heap(int size);

		bool isFull();

		xObjectNode* allocateNewObject();;

		void performGarbageCollection(SymbolTable* currentTable);

	};

}

#endif //POP_INTERPRETER_HEAP_H
