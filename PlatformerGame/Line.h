#pragma once
#include "headers.h"
using namespace std;

class Line
{
private:
	Point LineBounds[2];
	float cFriction;
public:
	Line();
	Line(Point start, Point end);
	Line(Point nLineBounds[]);

	Point getPoint(int index);
	void setPoint(Point newPoint, int index);

	void moveLine(float x, float y);
};

