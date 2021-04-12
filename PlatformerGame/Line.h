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

class Line
{
private:
	Point LineBounds[2];
	float cFriction;
public:
	Line();
	Line(Point start, Point end);
	Line(Point nLineBounds[]);

	double getLength();

	Point getPoint(int index);

	Point getPointAt(double v, char xy);

	Point getPointAt(double distance);

	void setPoint(Point newPoint, int index);

	bool intersectsLine(Line toCheck);

	Point whereIntersects(Line toCheck);

	void moveLine(float x, float y);
};

