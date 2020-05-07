#pragma once
#include "headers.h"
using namespace std;

InventoryNode::InventoryNode()
{

}

InventoryNode::InventoryNode(int index, InventoryNode nextNode, bool hasItem, Item contents)
{
	this->index = index;
	this->nextNode = &nextNode;
	this->hasItem = hasItem;
	this->contents = contents;
}

InventoryNode::~InventoryNode()
{
	delete nextNode;
	nextNode = nullptr;
}