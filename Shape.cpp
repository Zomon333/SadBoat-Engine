#pragma once
#include "headers.h"
using namespace std;

Shape::Shape(Line nBounds[])
{
	*this->bounds = nBounds;
	int i = 0;
	while (bounds[i] != NULL)
	{
		i++;
	}
	this->lineCount = i;
}

bool Shape::collidingWith(Shape toCheck)
{
	int i = 0;
	Point a,b;

	while (bounds[i] != nullptr)
	{
		a = bounds[i]->getStart();
		b = bounds[i]->getEnd();

		if (bounds[i]->checkIntersect(Line(a, b), TOLERANCE))
			return true;
		else
			if (a.insideShape(toCheck) || b.insideShape(toCheck))
				return true;
			else
				i++;
		if (bounds[i] == nullptr)
		{
			return false;
		}
	}
	return false;
}
