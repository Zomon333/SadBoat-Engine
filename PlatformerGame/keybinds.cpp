#pragma once
#include "headers.h"
using namespace std;

bool keybibbles::getKey(char key)
{
	if (key == 'w')
	{
		return wPressed;
	}
	if (key == 'a')
	{
		return aPressed;
	}
	if (key == 's')
	{
		return sPressed;
	}
	if (key == 'd')
	{
		return dPressed;
	}
}

void keybibbles::setKey(char key, bool value)
{
	if (key == 'w')
	{
		wPressed = value;
	}
	if (key == 'a')
	{
		aPressed = value;
	}
	if (key == 's')
	{
		sPressed = value;
	}
	if (key == 'd')
	{
		dPressed = value;
	}
}