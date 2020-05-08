#pragma once
#include "headers.h"
using namespace std;

Item::Item(string name, int id, bool usesDurability, float durabilityPercent)
{
	this->name = name;
	this->id = id;
	this->usesDurability = usesDurability;
	this->durabilityPercent = durabilityPercent;
}
