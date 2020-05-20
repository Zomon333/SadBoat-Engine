#pragma once
#include "headers.h"
using namespace std;

class Range
{
	float minimum;
	float maximum;
public:
	Range();
	Range(float A, float B);

	float getMinimum();
	float getMaximum();

	void setMinimum(float nMinimum);
	void setMaximum(float nMaximum);
	void setRanges(float A, float B);

	bool withinRange(float value);

};

