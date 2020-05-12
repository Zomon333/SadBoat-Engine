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

	bool containsPoint(Point toCheck, float tolerance);
	bool checkIntersect(Line toCheck, float tolerance);
	Point whereIntersect(Line toCheck);

	float getSlope();
	float getX(float Y);
	float getY(float X);

	void setStart(Point nStart);
	void setEnd(Point nEnd);
};

