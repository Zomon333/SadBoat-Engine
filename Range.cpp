#pragma once
#include "headers.h"
using namespace std;

float Range::getMinimum()
{
	return minimum;
}
float Range::getMaximum()
{
	return maximum;
}

void Range::setMinimum(float nMinimum)
{
	minimum = nMinimum;
}
void Range::setMaximum(float nMaximum)
{
	maximum = nMaximum;
}
void Range::setRanges(float A, float B)
{
	if (B > A)
	{
		maximum = B;
		minimum = A;
	}
	else
	{
		minimum = B;
		maximum = A;
	}
}

bool Range::withinRange(float value)
{
	if (value > minimum && value < maximum)
		return true;
	else return false;

}

Range::Range()
{
	minimum = 0;
	maximum = 180;
}
Range::Range(float A, float B)
{
	setRanges(A, B);
}