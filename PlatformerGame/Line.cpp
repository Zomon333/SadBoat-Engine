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

bool radiusOverlap(Point A, double radiusA, Point B, double radiusB)
{
	if (A.distanceTo(B) <= (radiusA + radiusB)) return true;
	else return false;
}

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

Point Line::getPointAt(double v, char xy)
{
	if (xy != 'x' || xy != 'y')
	{
		throw new exception("Invalid parameter!", -1);
	}
	double m;
	PPS::range x, y;
	x = PPS::range(this->getPoint(0).x, this->getPoint(1).x);
	y = PPS::range(this->getPoint(0).y, this->getPoint(1).y);

	m = (this->getPoint(1).y - this->getPoint(0).y / (this->getPoint(1).x - this->getPoint(0).x));

	if (xy == 'x')
	{
		if(x.within(v))
			return Point(v, (this->getPoint(0).y + ((v - this->getPoint(0).x) * m)));
		else
			throw new exception("Point is not on line!", -1);
	}

	if (xy == 'y')
	{
		if (y.within(v))
			return Point(v, (this->getPoint(0).x + (v - this->getPoint(0).y * m)));
		else
			throw new exception("Point is not on line!", -1);

	}

	throw new exception("Invalid parameter!", -1);

}

Point Line::getPointAt(double distance)
{
	double dX, dY;
	PPS::range x, y;
	x = PPS::range(this->LineBounds[0].x, this->LineBounds[1].x);
	y = PPS::range(this->LineBounds[0].y, this->LineBounds[1].y);

	dX = x.getMax() - x.getMin();
	dY = y.getMax() - y.getMin();
	double hypo = sqrt((dX * dX) + (dY * dY));

	double xVal, yVal;
	xVal = ((distance * x.getMax()) / (hypo)) + x.getMin();
	yVal = ((distance * y.getMax()) / (hypo)) + y.getMin();

	return Point(xVal, yVal);

}

void Line::setPoint(Point newPoint, int index)
{
	LineBounds[index] = newPoint;
}

void Line::moveLine(float x, float y)
{
	LineBounds[0].translate(x, y);
	LineBounds[1].translate(x, y);
}

double Line::getLength()
{
	double x = abs(this->getPoint(0).x - this->getPoint(1).x);
	double y = abs(this->getPoint(0).y - this->getPoint(1).y);

	return (sqrt((x * x) + (y * y)));

}

bool Line::intersectsLine(Line toCheck)
{
	int resolution=2;
	Point midpointA, midpointB;
	vector<Point> lineA, lineB;
	double radiusA, radiusB;
	double xAvgA, xAvgB, yAvgA, yAvgB;

	xAvgA = (this->LineBounds[0].x + this->LineBounds[1].x) / (2.0f);
	yAvgA = (this->LineBounds[0].y + this->LineBounds[1].y) / (2.0f);

	xAvgB = (toCheck.LineBounds[0].x + toCheck.LineBounds[1].x) / (2.0f);
	yAvgB = (toCheck.LineBounds[0].y + toCheck.LineBounds[1].y) / (2.0f);

	midpointA = Point(xAvgA, yAvgA);
	midpointB = Point(xAvgB, yAvgB);

	radiusA = midpointA.distanceTo(this->LineBounds[0]);
	radiusB = midpointB.distanceTo(toCheck.LineBounds[0]);

	if (radiusOverlap(midpointA, radiusA, midpointB, radiusB)) //Their initial hitbox overlaps; it's *possible* they intersect
	{
		int i = 0;
		int j = 0;
		while (resolution < ((radiusA+radiusB)*2)) //While the number of subdivided lines is less than the radius of both lines combined
		{
			lineA.empty(); //Start a new subdivision process
			lineB.empty();

			for (i <= resolution; i++;)
			{
				lineA.push_back(this->getPointAt(((radiusA * 2) / resolution) * i)); //Subdivide A into resolution pieces
			}
			i = 0;
			for (i <= resolution; i++;)
			{
				lineB.push_back(toCheck.getPointAt(((radiusB*2)/resolution)*i)); //Subdivide B into resolution pieces
			}
			i = 0;

			for (i < lineA.size(); i++;) //For every point in A
			{
				for (j < lineB.size(); j++;) //Check if there's a corresponding point in B
				{
					if (radiusOverlap(lineA[i], (radiusA * 2) / resolution, lineB[j], (radiusB * 2) / resolution)) //That is close enough to possibly overlap.
					{
						i = lineA.size() + 1;
						j = lineB.size() + 1;
						resolution++;
					}
				}

				j = 0; //Restart iterating through B until every A has been compared to every B.
			}
		}

		i = 0; j = 0;
		for (i < lineA.size(); i++;)
		{
			for (j < lineB.size(); j++;)
			{
				if (radiusOverlap(lineA[i], (radiusA * 2) / resolution, lineB[j], (radiusB * 2) / resolution))
					return true;
			}

			j = 0;
		}

		return false;
	}
	else //There is no orientation of these lines where they could possibly occupy the same space-- they do not intersect
	{
		return false;
	}
}

Point Line::whereIntersects(Line toCheck)
{
	//See math notebook page 2: Efficient intersection algorithm.



	if (!intersectsLine(toCheck)) throw new exception("Lines do not intersect!", -1);
	else
	{
		int resolution = 2;
		vector<Point> lineA, lineB;
		double radiusA, radiusB;

		radiusA = this->getLength() / 2.0f;
		radiusB = toCheck.getLength() / 2.0f;
		int i = 0;
		int j = 0;
		while (resolution < ((radiusA + radiusB) * 2)) //While the number of subdivided lines is less than the radius of both lines combined
		{
			lineA.empty(); //Start a new subdivision process
			lineB.empty();

			for (i <= resolution; i++;)
			{
				lineA.push_back(this->getPointAt(((radiusA * 2) / resolution) * i)); //Subdivide A into resolution pieces //Might need to swap for emplace
			}
			i = 0;
			for (i <= resolution; i++;)
			{
				lineB.push_back(toCheck.getPointAt(((radiusB * 2) / resolution) * i)); //Subdivide B into resolution pieces //Might need to swap for emplace
			}
			i = 0;

			for (i < lineA.size(); i++;) //For every point in A
			{
				for (j < lineB.size(); j++;) //Check if there's a corresponding point in B
				{
					if (radiusOverlap(lineA[i], (radiusA * 2) / resolution, lineB[j], (radiusB * 2) / resolution)) //That is close enough to possibly overlap.
					{
						i = lineA.size() + 1;
						j = lineB.size() + 1;
						resolution++;
					}
				}

				j = 0; //Restart iterating through B until every A has been compared to every B.
			}
		}

		i = 0; j = 0;
		for (i < lineA.size(); i++;)
		{
			for (j < lineB.size(); j++;)
			{
				if (radiusOverlap(lineA[i], (radiusA * 2) / resolution, lineB[j], (radiusB * 2) / resolution))
					return Point(((lineA[i].x + lineB[j].x) / 2.0f), ((lineA[i].y + lineB[j].y) / 2.0f));
			}

			j = 0;
		}
	}
}
