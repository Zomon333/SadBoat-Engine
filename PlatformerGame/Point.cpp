#pragma once
#include "headers.h"
using namespace std;

Point::Point()
{
	x = 0;
	y = 0;
	//cout << "E\n";
}

Point::Point(float x, float y)
{
	this->x = x;
	this->y = y;
}

void Point::movePoint(float x, float y)
{
	this->x += x;
	this->y += y;
}

float Point::distanceTo(Point toCheck)
{
	float largestX, largestY;
	float smallestX, smallestY;
	float deltaX, deltaY;
	float distance;

	largestX = 0;
	largestY = 0;
	smallestX = 0;
	smallestY = 0;
	deltaX = 0;
	deltaY = 0;
	distance = 0;

	if (this->x > toCheck.x)
	{
		largestX = this->x;
		smallestX = toCheck.x;
	}
	else
	{
		largestX = toCheck.x;
		smallestX = this->x;
	}
	if (this->y > toCheck.y)
	{
		largestX = this->y;
		smallestX = toCheck.y;
	}
	else
	{
		largestX = toCheck.y;
		smallestX = this->y;
	}

	deltaX = largestX - smallestX;
	deltaY = largestY - smallestY;

	distance = sqrt((deltaX*deltaX)+(deltaY*deltaY));
	return distance;
}


