#pragma once
#include "headers.h"
using namespace std;

Point::Point()
{
	x = 0.0;
	y = 0.0;
}

Point::Point(float x, float y)
{
	this->x = x;
	this->y = y;
}


bool Point::insideShape(Shape toCheck)
{
	Point pointToCheck = Point(getX(), getY());
	Point xStart = Point(pointToCheck.getX(), pointToCheck.getY());
	Point xEnd = Point(pointToCheck.getX(), (pointToCheck.getY() + 1000000));
	Line xBounds = Line(xStart, xEnd);

	Point yStart = Point(pointToCheck.getX(), pointToCheck.getY());
	Point yEnd = Point((pointToCheck.getX() + 1000000), pointToCheck.getY());
	Line yBounds = Line(yStart, yEnd);

	string xVal, yVal;

	int i = 0;
	int intersectsX = 0;
	int intersectsY = 0;
	Line workingLine;
	while (i<toCheck.lineCount)
	{
			workingLine = *toCheck.bounds[i];
			if (xBounds.checkIntersect(workingLine, TOLERANCE))
			{
				cout << "A X intersect occured!\n";
				intersectsX++;;
			}
			if (yBounds.checkIntersect(workingLine, TOLERANCE))
			{
				cout << "A Y intersect occured!\n";
				intersectsY++;
			}
			i++;
		

	}
	if (intersectsX % 2 == 1)
	{
		
		xVal = "ODD";
		cout << "xVal = " << xVal;
	}
	else
	{
		xVal = "EVEN";
		cout << "xVal = " << xVal;
	}

	if (intersectsY % 2 == 1)
	{
		yVal = "ODD";
		cout << "yVal = " << xVal;
	}
	else
	{
		yVal = "EVEN";
		cout << "yVal = " << xVal;
	}

	if (xVal == yVal && xVal == "ODD")
	{
		return true;
	}
	if (xVal == yVal && xVal == "EVEN")
	{
		return false;
	}
	if (xVal != yVal)
	{
		//throw new exception("Raycasting failed, xRay and yRay held different results. Recheck your parameters and try again.", 0);
	}
	return false;
}

