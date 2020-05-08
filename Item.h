#pragma once
#include "headers.h"
using namespace std;

class Item
{
private:
	string name;
	int id;
	int amount;
	bool usesDurability;
	float durabilityPercent;

public:
	Item(string name = "", int id = 0, bool usesDurability = false, float durabilityPercent = 1);

	string getName() { return name; }
	void setName(string nName) { name = nName; }

	bool getDurabilityUse() { return usesDurability; }
	void setDurabilityUse(bool nUsesDurability) { usesDurability = nUsesDurability; }

	void setDurabilityPercent(float nDurabilityPercent) { durabilityPercent = nDurabilityPercent; }
	float getDurabilityPercent() { return durabilityPercent; }
	void damageDurability(float amount) { durabilityPercent -= durabilityPercent; }

	int getAmount() { return amount; }
	void setAmount(int nAmount) { amount = nAmount; }
};

