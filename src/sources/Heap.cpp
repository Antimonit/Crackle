#include "headers/Heap.hpp"
#include <c++/iostream>

#include "nodes/DataType.h"

#include "nodes/xObjectNode.h"
#include "nodes/xVariableNode.h"
#include "nodes/xConstantNode.h"
#include "nodes/constant/xConstantObjectNode.h"

MC::Heap::Heap(int size) {
	this->size = size;
	this->allocatedObjects = 0;
	this->objects = new xObjectNode*[size];
	for (int i = 0; i < size; ++i) {
		this->objects[i] = nullptr;
	}
}

bool MC::Heap::isFull() {
	return allocatedObjects == size;
}

xObjectNode* MC::Heap::allocateNewObject() {
	int heapIndex = findFreeSlotInHeap();

	if (heapIndex == -1) {
		std::cerr << "Didn't find a free slot in heap" << std::endl;
		return nullptr;
	}

	auto* newData = new xObjectNode;

	objects[heapIndex] = newData;
	allocatedObjects++;

	newData->marked = false;

	return newData;
}

void MC::Heap::performGarbageCollection(MC::SymbolTable* currentTable) {
	std::cout << "===== Performing GC =====" << std::endl;
	std::cout << "AllocatedObjects: " << allocatedObjects << std::endl;
	gcMarkAllTables(currentTable);
	gcSweep();
	std::cout << "AllocatedObjects: " << allocatedObjects << std::endl;
	std::cout << "===== Performed GC =====" << std::endl;
}

int MC::Heap::findFreeSlotInHeap() {
	for (int i=0; i < size; i++) {
		if (objects[i] == nullptr) {
			return i;
		}
	}
	return -1;
}

void MC::Heap::gcMarkAllTables(MC::SymbolTable* table) {
	auto current = table;
	while (current != nullptr) {
		gcMarkTable(current);
		current = current->previousTable;
	}
}

void MC::Heap::gcMarkTable(MC::SymbolTable* table) {
	for (auto item : table->variables) {
		auto rootValue = item.second->value;
		if (rootValue->getType() == typeObject) {
			gcMarkNode(rootValue->toObject()->objectVal);
		}
	}
}

void MC::Heap::gcMarkNode(xObjectNode* object) {
	if (object != nullptr && !object->marked) {
		object->marked = true;

		for (auto childVar : object->vars) {
			if (childVar->value->getType() == typeObject) {
				gcMarkNode(childVar->value->toObject()->objectVal);
			}
		}
	}
}

void MC::Heap::gcSweep() {
	for (int i = 0; i < size; i++) {
		xObjectNode* object = objects[i];

		if (!object->marked) {
			delete object;
			objects[i] = nullptr;
			allocatedObjects--;
		} else {
			object->marked = false;
		}
	}
}
