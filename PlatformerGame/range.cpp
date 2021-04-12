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

	bool range::within(double value)
	{
		if (value >= min && value <= max)
			return true;
		else return false;
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