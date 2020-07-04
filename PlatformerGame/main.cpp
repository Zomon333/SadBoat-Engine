#pragma once
#include "headers.h"
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

	PPS::Game game;
	int response = game.exec(game);
	
	ShowWindow(Console, true);

	sleep(3000);
	return response;
}