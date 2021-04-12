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
		
		bool isWithin(Point A);
		void translate(double x, double y);
	};
}


