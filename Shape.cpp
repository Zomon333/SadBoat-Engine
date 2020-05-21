#pragma once
#include "headers.h"
using namespace std;

Line* Polygon::getBounds()
{
	return *(this->bounds);
}

void Polygon::setBounds(Line nBounds[])
{
	*this->bounds = nBounds;
}

int Polygon::getLineCount()
{
	return this->lineCount;
}

void Polygon::setLineCount(int nLineCount)
{
	this->lineCount = nLineCount;
}

Polygon::Polygon(Line nBounds[])
{
	*this->bounds = nBounds;
	int i = 0;
	while (bounds[i] != NULL)
	{
		i++;
	}
	this->lineCount = i;
}

/*
bool Polygon::collidingWith(Polygon toCheck)
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
}*/



