#pragma once
#include "headers.h"
using namespace std;

static class keybibbles
{
private:
	bool wPressed;
	bool aPressed;
	bool sPressed;
	bool dPressed;
public:
	keybibbles()
	{
		wPressed = false;
		aPressed = false;
		sPressed = false;
		dPressed = false;
	}
	bool getKey(char key);
	
	void setKey(char key, bool value);
	
};

