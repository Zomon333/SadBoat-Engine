#pragma once
#include "headers.h"
using namespace std;

class Inventory
{
private:
	InventoryNode *firstNode;
	InventoryNode *lastNode;

	int length;
public:
	Inventory();
	Inventory(InventoryNode* firstNode, InventoryNode* lastNode, int length=0);
	~Inventory();

	void generateInventory();
	void extendInventory(int lengthToAdd);
	void shrinkInventory(int lengthToRemove);

	InventoryNode getNodeIndex(int index);
	void setNodeIndex(InventoryNode newNode, int index);

};

