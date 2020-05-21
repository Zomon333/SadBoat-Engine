#pragma once
#include "headers.h"
using namespace std;
using namespace sf;

class Line
{
private:
	Point start;
	Point end;
	float m;
	bool checkIntersectSingular(Line toCheck, float tolerance); // A unilateral intersect checker

public:
	Line();
	Line(Point nStart, Point nEnd);

	bool containsPoint(Point toCheck, float tolerance);
	bool containsPoint(Point toCheck);

	float FPSRaycast(float angle, string direction, Line toCheck[], int sides); // Returns distance from collision
	
	Point whereIntersect(Line toCheck);
	Point lineAverage();

	bool checkIntersect(Line toCheck, float tolerance); // A bilateral intersect checker, using the unilateral checker twice.
	float getSlope();
	float getX(float Y);
	float getY(float X);

	Point getStart();
	Point getEnd();

	void setStart(Point nStart);
	void setEnd(Point nEnd);
};

