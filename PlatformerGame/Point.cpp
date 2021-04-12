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

Point::Point()
{
	x = 0;
	y = 0;
	//cout << "E\n";
}

Point::Point(double x, double y)
{
	this->x = x;
	this->y = y;
}

void Point::translate(double x, double y)
{
	this->x += x;
	this->y += y;
}

float Point::distanceTo(Point toCheck)
{
	double largestX, largestY;
	double smallestX, smallestY;
	double deltaX, deltaY;
	double distance;

	largestX = 0;
	largestY = 0;
	smallestX = 0;
	smallestY = 0;
	deltaX = 0;
	deltaY = 0;
	distance = 0;

	if (this->x > toCheck.x)
	{
		largestX = this->x;
		smallestX = toCheck.x;
	}
	else
	{
		largestX = toCheck.x;
		smallestX = this->x;
	}
	if (this->y > toCheck.y)
	{
		largestX = this->y;
		smallestX = toCheck.y;
	}
	else
	{
		largestX = toCheck.y;
		smallestX = this->y;
	}

	deltaX = largestX - smallestX;
	deltaY = largestY - smallestY;

	distance = sqrt((deltaX*deltaX)+(deltaY*deltaY));
	return distance;
}

bool Point::operator==(Point toCheck)
{
	if (x == toCheck.x && y == toCheck.y)
		return true;
	else return false;
}


