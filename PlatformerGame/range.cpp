#pragma once
#include "headers.h"
using namespace std;

namespace PPS
{

	range::range()
	{
		min = 0;
		max = 0;
	}

	range::range(float A, float B)
	{
		if (A > B)
		{
			max = A;
			min = B;
		}
		else
		{
			max = B;
			min = A;
		}
	}

	bool range::operator<(double value)
	{
		if (value < min && value < max)
			return true;
		else return false;
	}

	bool range::operator>(double value)
	{
		if (value > max&& value > min)
			return true;
		else return false;
	}

	bool range::operator==(double value)
	{
		if (value > min && value < max) 
			return true;
		else return false;
	}

}