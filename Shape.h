#pragma once
#include "headers.h"
using namespace std;

class Shape
{
public:
	int lineCount;
	Line* bounds[];

	Shape(Line nBounds[]);
	bool collidingWith(Shape toCheck);
};

