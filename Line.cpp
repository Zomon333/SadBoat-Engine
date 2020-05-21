#pragma once
#include "headers.h"
using namespace std;



Line::Line()
{
	m = getSlope();
}

Line::Line(Point nStart, Point nEnd)
{
	setStart(nStart);
	setEnd(nEnd);
	m = getSlope();
}


bool Line::containsPoint(Point toCheck)
{
	float tolerance = TOLERANCE;
	if ((toCheck.getY() - (getSlope() * toCheck.getX())) > (((-1 * getSlope() * start.getX()) + start.getY()) - tolerance) && (toCheck.getY() - (getSlope() * toCheck.getX())) < (((-1 * getSlope() * start.getX()) + start.getY()) + tolerance) || (toCheck.getY() - (getSlope() * toCheck.getX())) == (((-1 * getSlope() * start.getX()) + start.getY())))
		return true;
	else
		return false;

}



bool Line::containsPoint(Point toCheck, float tolerance=TOLERANCE)
{
	
	if ((toCheck.getY() - (getSlope() * toCheck.getX())) > (((-1 * getSlope() * start.getX()) + start.getY())-tolerance) && (toCheck.getY() - (getSlope() * toCheck.getX())) < (((-1 * getSlope() * start.getX()) + start.getY()) + tolerance))
		return true;
	else
		return false;
		
}

Point Line::whereIntersect(Line toCheck)
{
	Point intersectPoint;
	float m1, x1, y1, m2, x2, y2;
	float x, y;

	m1 = toCheck.getSlope();
	m2 = getSlope();

	x1 = toCheck.start.getX();
	y1 = toCheck.start.getY();

	x2 = start.getX();
	y2 = start.getY();

	x = ((((m2 * x2) + y2 - y1 + (m1 * x1)) / ((-1 * m1) + m2)) * -1);
	y = getY(x);

	intersectPoint.setX(x);
	intersectPoint.setY(y);

	return intersectPoint;
}

Point Line::lineAverage()
{
	float startX = this->start.getX();
	float startY = this->start.getY();
	float endX = this->end.getX();
	float endY = this->end.getY();

	return Point(((startX + endX) / 2), ((startY + endY) / 2));
}

bool Line::checkIntersect(Line toCheck, float tolerance)
{
	if (this->checkIntersectSingular(toCheck, TOLERANCE) || toCheck.checkIntersectSingular(Line(getStart(), getEnd()), TOLERANCE))
		return true;
	return false;
}

bool Line::checkIntersectSingular(Line toCheck, float tolerance)
{
	Point intersectPoint;
	float m1, x1, y1, m2, x2, y2;
	float x, y;

	m1 = toCheck.getSlope();
	m2 = getSlope();
	//cout << "m1, m2: " << m1 << ", " << m2 << endl;

	x1 = toCheck.start.getX();
	y1 = toCheck.start.getY();
	//cout << "x1,y1: " << x1 << ", " << y1 << endl;

	x2 = start.getX();
	y2 = start.getY();

	//cout << "x2,y2: " << x2 << ", " << y2 << endl;

	x = ((((m2 * x2) + y2 - y1 + (m1 * x1)) / ((-1 * m1) + m2)) * -1);
	y = getY(x);

	//cout << "Intersect at: " << x << ", " << y << endl;
	intersectPoint = Point(x, y);

	cout << "Line 1 has point? " << this->containsPoint(intersectPoint, TOLERANCE) << endl;
	cout << "Line 2 has point? " << toCheck.containsPoint(intersectPoint, TOLERANCE) << endl;

	if (this->containsPoint(intersectPoint, TOLERANCE) && toCheck.containsPoint(intersectPoint, TOLERANCE))
		return true;
	else return false;



}


float Line::getSlope()
{
	float dX, dY;
	dX = end.getX() - start.getX();
	dY = end.getY() - start.getY();
	if (dX == 0 || dY == 0)
	{
		this->m = 0;
		return 0;
	}
	else
	{
		this->m = (dY / dX);
		return (dY / dX);
	}
	return m;
}

float Line::getX(float Y)
{
	float X;
	X = ((Y - start.getY()) / getSlope()) - start.getX();
	return X;
}
float Line::getY(float X)
{
	float Y;
	Y = (getSlope() * (X - start.getX())) + start.getY();
	return Y;
}
Point Line::getStart()
{
	return this->start;
}
Point Line::getEnd()
{
	return this->end;
}
void Line::setStart(Point nStart)
{
	this->start = nStart;
}

void Line::setEnd(Point nEnd)
{
	this->end = nEnd;
}
