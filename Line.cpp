#pragma once
#include "headers.h"
using namespace std;

float radToDegree(float radians)
{
	return (radians * 57.2958);
}

float degreeToRad(float degrees)
{
	return (degrees / 57.2958);
}

Line::Line()
{
	m = getSlope();
}

bool Line::containsPoint(Point toCheck, float tolerance)
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

bool Line::checkIntersect(Line toCheck, float tolerance)
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

	if (toCheck.getY(x) > getY(x) - tolerance && toCheck.getY(x) < getY(x) + tolerance && toCheck.getX(getY(x)) > getX(getY(x)) - tolerance && toCheck.getX(getY(x)) < getX(getY(x)) + tolerance)
	{
		return true;
	}
	else
		return false;

}

float Line::getSlope()
{
	float dX, dY;
	dX = end.getX() - start.getX();
	dY = end.getY() - start.getY();
	if(dX!=0)
	this->m = dY / dX;
	else
	{
		m = 9999999999999999999;
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
void Line::setStart(Point nStart)
{
	this->start = nStart;
}

void Line::setEnd(Point nEnd)
{
	this->end = nEnd;
}