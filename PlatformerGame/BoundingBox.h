#pragma once
#include "headers.h"
using namespace std;

namespace PPS
{
	class BoundingBox
	{
	private:
		range X;
		range Y;
		Point corners[2];
	public:
		BoundingBox();
		BoundingBox(Point A, Point B);

		bool operator==(Point toCheck);
		
		void translate(double x, double y);
	};
}


