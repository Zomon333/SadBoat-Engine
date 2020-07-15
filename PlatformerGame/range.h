#pragma once
#include "headers.h"
using namespace std;
namespace PPS
{
	class range
	{
	private:
		double min, max;
	public:
		range();
		range(float A, float B);

		bool operator<(double value);
		bool operator>(double value);
		bool operator==(double value);
	};
}

