#pragma once
#include "headers.h"
using namespace std;

class Line
{
private:
	Point start;
	Point end;
	float m;
public:
	Line();
	Line(Point nStart, Point nEnd);

	bool containsPoint(Point toCheck, float tolerance);
	bool containsPoint(Point toCheck);

	bool checkIntersect(Line toCheck, float tolerance);
	Point whereIntersect(Line toCheck);

	float getSlope();
	float getX(float Y);
	float getY(float X);

	Point getStart();
	Point getEnd();

	void setStart(Point nStart);
	void setEnd(Point nEnd);
};

