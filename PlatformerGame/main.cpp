#pragma once
#include "headers.h"
using namespace std;

void sleep(int seconds)
{
	cout << "\nSleeping for " << seconds << " seconds.\n";
	chrono::steady_clock::duration duration = chrono::seconds(seconds);
	chrono::steady_clock::time_point start = chrono::steady_clock::now();

	do
	{

	} while (chrono::steady_clock::now()<(start+duration));
}


int main()
{
	PPS::Game game;
	int response = game.exec();

	sleep(3);
	return response;
}


