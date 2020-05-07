#pragma once
#include "headers.h"
using namespace std;

Item::Item(string name = "", int id = 0, bool usesDurability = false, float durabilityPercent = 1)
{
	this->name = name;
	this->id = id;
	this->usesDurability = usesDurability;
	this->durabilityPercent = durabilityPercent;
}
