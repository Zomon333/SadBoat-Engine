#pragma once
#include "headers.h"
using namespace std;
class Shape;

class Point
{
private:
	float x;
	float y;
public:
	Point();
	Point(float x, float y);

	float getX() { return this->x; }
	void setX(float x) { this->x = x; }

	float getY() { return this->y; }
	void setY(float y) { this->y = y; }

	bool insideShape(Shape toCheck);

};

