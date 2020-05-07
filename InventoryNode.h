#pragma once
#include "headers.h"
using namespace std;

class InventoryNode
{
private:
	int index;
	InventoryNode* nextNode;

	bool hasItem;
	Item contents;

public:
	InventoryNode();
	InventoryNode(int index, InventoryNode nextNode, bool hasItem, Item contents);
	~InventoryNode();

	bool isEmpty() { return !hasItem; }
	void setHasItem(bool nHasItem) { hasItem = nHasItem; }

	int getIndex() { return index; }
	void setIndex(int nIndex) { index = nIndex; }

	InventoryNode* getNextNode() { return nextNode; }
	void setNextNode(InventoryNode &nNextNode) { nextNode = &nNextNode; }

	Item getContents() { return contents; }
	void setContents(Item nContents) { contents = nContents; }

};

