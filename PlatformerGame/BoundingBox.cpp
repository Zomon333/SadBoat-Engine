#pragma once
#include "headers.h"
/*
Dagan Poulin, SadBoat Entertainment, 4/12/2021
 Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
using namespace std;

PPS::BoundingBox::BoundingBox()
{
	BoundingBox(Point(0,0), Point(1,1));
}

PPS::BoundingBox::BoundingBox(Point A, Point B)
{
	corners[0] = A;
	corners[1] = B;

	double minX=0, maxX=0;
	double minY=0, maxY=0;

	if (A.x > B.x)
	{
		maxX = A.x;
		minX = B.x;
	}
	else
	{
		if (A.x < B.x)
		{
			maxX = B.x;
			minX = B.x;
		}
	}
	X = range(minX, maxX);

	if (A.y > B.y)
	{
		maxY = A.y;
		minY = B.y;
	}
	else
	{
		if (A.y < B.y)
		{
			maxY = B.y;
			minY = B.y;
		}
	}
	Y = range(minY, maxY);
}

bool PPS::BoundingBox::operator==(Point toCheck)
{
	if(X==toCheck.x)
		if (Y == toCheck.y)
			return true;
		else return false;
	else return false;

	return false;
}

bool PPS::BoundingBox::isWithin(Point A)
{
	return (X.within(A.x) && Y.within(A.y));
}

void PPS::BoundingBox::translate(double x, double y)
{
	corners[0].translate(x, y);
	corners[1].translate(x, y);
	BoundingBox(corners[0], corners[1]);
}
