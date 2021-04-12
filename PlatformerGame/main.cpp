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

void sleep(int milliseconds)
{
	cout << "\nSleeping for " << milliseconds << " milliseconds. (" << (( (float)(milliseconds) ) / 1000.0 ) << " s)\n";
	chrono::steady_clock::duration duration = chrono::milliseconds(milliseconds);
	chrono::steady_clock::time_point start = chrono::steady_clock::now();

	do
	{

	} while (chrono::steady_clock::now()<(start+duration));
}


int main()
{
	
	Console = GetConsoleWindow();

	ShowWindow(Console, false);

	PPS::Game game("Presidential Physics Simulator");
	int response = game.exec(game);

	ShowWindow(Console, true);

	sleep(3000);
	return response;
}