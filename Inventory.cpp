#pragma once
#include "headers.h"
using namespace std;

Inventory::Inventory()
{

}

Inventory::Inventory(InventoryNode* firstNode, InventoryNode* lastNode, int length = 0)
{
	this->firstNode = firstNode;
	this->lastNode = lastNode;
	this->length = length;
}

Inventory::~Inventory()
{
	int i = 0;
	InventoryNode* temp;
	while (i < length)
	{
		temp = firstNode->getNextNode();
		firstNode->~InventoryNode();
		delete firstNode;
		firstNode = temp;
		i++;
		if (firstNode == lastNode)
		{
			i = length;
		}
	}
}

void Inventory::generateInventory()
{
	int i = 0;
	firstNode = new InventoryNode();
	while (i < length)
	{
		
	}

}

void Inventory::extendInventory(int lengthToAdd)
{

}

void Inventory::shrinkInventory(int lengthToRemove)
{

}

InventoryNode Inventory::getNodeIndex(int index)
{

}

void Inventory::setNodeIndex(InventoryNode newNode, int index)
{

}
