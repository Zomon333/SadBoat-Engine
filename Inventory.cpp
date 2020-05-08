#pragma once
#include "headers.h"
using namespace std;

Inventory::Inventory()
{
	this->firstNode = new InventoryNode;
	this->lastNode = firstNode;
	this->length = 0;
}

Inventory::Inventory(InventoryNode* firstNode, InventoryNode* lastNode, int length)
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
	firstNode = new InventoryNode;
	firstNode->setIndex(i);
	InventoryNode* tmpA;
	InventoryNode* tmpB;

	tmpA = firstNode;
	i++;
	try
	{
		while (i < length)
		{
			tmpB = new InventoryNode;
			tmpB->setIndex(i);
			tmpA->setNextNode(*tmpB);
			delete tmpB;
			tmpB = tmpA->getNextNode();
			tmpA = tmpB;
			delete tmpB;
			i++;
		}
		delete tmpA;
	}
	catch (exception*)
	{
		cout << "You did something wrong. generateInventory();";
	}
}

void Inventory::extendInventory(int lengthToAdd)
{
	InventoryNode* tmpExtender;
	InventoryNode* workingMemory;
	int i = 0;
	tmpExtender = firstNode;
	try 
	{
		while (i < lengthToAdd)
		{
			if (tmpExtender->getNextNode() != nullptr)
				tmpExtender = tmpExtender->getNextNode();
			else
			{
				workingMemory = new InventoryNode;
				workingMemory->setIndex(tmpExtender->getIndex() + 1);
				tmpExtender->setNextNode(*workingMemory);
				delete workingMemory;
				i++;
			}
		}
	}
	catch (exception*)
	{
		cout << "You did something wrong. extendInventory();";
	}
}

void Inventory::shrinkInventory(int lengthToRemove)
{
	InventoryNode* workingMemory;
	workingMemory = firstNode;
	int i = 0;
	try
	{
		while (i < lengthToRemove)
		{
			if (workingMemory->getNextNode() != nullptr)
				workingMemory = workingMemory->getNextNode();
			else
			{
				delete workingMemory;
				workingMemory = firstNode;
				i++;
			}
		}
	}
	catch (exception*)
	{
		cout << "You did something wrong. shrinkInventory();";
	}
}

InventoryNode Inventory::getNodeIndex(int index)
{
	InventoryNode* workingMemory;
	workingMemory = firstNode;
	while (workingMemory->getIndex() != index)
	{
		if(workingMemory->getNextNode() != nullptr)
			workingMemory = workingMemory->getNextNode();
		else
		{
			break;
		}
	}
	if (workingMemory->getIndex() == index)
		return *workingMemory;
	else
		throw new out_of_range::exception;
}

void Inventory::setNodeIndex(InventoryNode newNode, int index)
{
	InventoryNode* prevNode;
	InventoryNode* replaceNode;
	InventoryNode* nextNode;
	prevNode = &getNodeIndex(index - 1);
	if (prevNode->getNextNode() != nullptr)
		replaceNode = &getNodeIndex(index);
	else
		throw new exception;
	if (replaceNode->getNextNode() != nullptr)
		nextNode = replaceNode->getNextNode();
	else
		throw new exception;
	delete replaceNode;
	replaceNode = &newNode; //This might become a problem
	replaceNode->setNextNode(*nextNode);
	prevNode->setNextNode(*replaceNode);
}
