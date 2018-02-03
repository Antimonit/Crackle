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
		ObjectNode** objects;

		int findFreeSlotInHeap();;

		void gcMarkAllTables(SymbolTable* table);

		void gcMarkTable(SymbolTable* table);

		void gcMarkNode(ObjectNode* object);

		void gcSweep();

	public:

		explicit Heap(int size);

		bool isFull();

		ObjectNode* allocateNewObject();;

		void performGarbageCollection(SymbolTable* currentTable);

	};

}

#endif //POP_INTERPRETER_HEAP_H
