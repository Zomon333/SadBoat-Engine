#pragma once
#include "headers.h"
using namespace std;

Line::Line()
{
	LineBounds[0] = Point();
	LineBounds[1] = Point();
}

Line::Line(Point start, Point end)
{
	LineBounds[0] = start;
	LineBounds[1] = end;
}

Line::Line(Point nLineBounds[])
{
	LineBounds[0] = nLineBounds[0];
	LineBounds[1] = nLineBounds[1];
}

Point Line::getPoint(int index)
{
	return LineBounds[index];
}

void Line::setPoint(Point newPoint, int index)
{
	LineBounds[index] = newPoint;
}

void Line::moveLine(float x, float y)
{
	LineBounds[0].movePoint(x, y);
	LineBounds[1].movePoint(x, y);
}
