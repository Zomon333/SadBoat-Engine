#pragma once
#include "headers.h"
using namespace std;
using namespace sf;

class colormap
{
public:
	RectangleShape map[10][10];
	int r = 0;
	colormap()
	{
		map[0][r].setFillColor(Color::Red);
		map[1][r].setFillColor(Color::Red);
		map[2][r].setFillColor(Color::Red);
		map[3][r].setFillColor(Color::Red);
		map[4][r].setFillColor(Color::Red);
		map[5][r].setFillColor(Color::Red);
		map[6][r].setFillColor(Color::Red);
		map[7][r].setFillColor(Color::Red);
		map[8][r].setFillColor(Color::Red);
		map[9][r].setFillColor(Color::Red);

		//Row 2
		r = 1;
		map[0][r].setFillColor(Color::Red);
		map[1][r].setFillColor(Color::Transparent);
		map[2][r].setFillColor(Color::Transparent);
		map[3][r].setFillColor(Color::Transparent);
		map[4][r].setFillColor(Color::Transparent);
		map[5][r].setFillColor(Color::Transparent);
		map[6][r].setFillColor(Color::Transparent);
		map[7][r].setFillColor(Color::Transparent);
		map[8][r].setFillColor(Color::Transparent);
		map[9][r].setFillColor(Color::Red);

		//Row 3
		r = 2;
		map[0][r].setFillColor(Color::Red);
		map[1][r].setFillColor(Color::Transparent);
		map[2][r].setFillColor(Color::Green);
		map[3][r].setFillColor(Color::Green);
		map[4][r].setFillColor(Color::Green);
		map[5][r].setFillColor(Color::Green);
		map[6][r].setFillColor(Color::Green);
		map[7][r].setFillColor(Color::Transparent);
		map[8][r].setFillColor(Color::Green);
		map[9][r].setFillColor(Color::Red);

		//Row 4
		r = 3;
		map[0][r].setFillColor(Color::Red);
		map[1][r].setFillColor(Color::Transparent);
		map[2][r].setFillColor(Color::Transparent);
		map[3][r].setFillColor(Color::Transparent);
		map[4][r].setFillColor(Color::Transparent);
		map[5][r].setFillColor(Color::Transparent);
		map[6][r].setFillColor(Color::Transparent);
		map[7][r].setFillColor(Color::Transparent);
		map[8][r].setFillColor(Color::Transparent);
		map[9][r].setFillColor(Color::Red);

		//Row 5
		r = 4;
		map[0][r].setFillColor(Color::Red);
		map[1][r].setFillColor(Color::Transparent);
		map[2][r].setFillColor(Color::Green);
		map[3][r].setFillColor(Color::Green);
		map[4][r].setFillColor(Color::Green);
		map[5][r].setFillColor(Color::Green);
		map[6][r].setFillColor(Color::Green);
		map[7][r].setFillColor(Color::Transparent);
		map[8][r].setFillColor(Color::Transparent);
		map[9][r].setFillColor(Color::Red);

		//Row 6
		r = 5;
		map[0][r].setFillColor(Color::Red);
		map[1][r].setFillColor(Color::Transparent);
		map[2][r].setFillColor(Color::Transparent);
		map[3][r].setFillColor(Color::Transparent);
		map[4][r].setFillColor(Color::Transparent);
		map[5][r].setFillColor(Color::Transparent);
		map[6][r].setFillColor(Color::Green);
		map[7][r].setFillColor(Color::Transparent);
		map[8][r].setFillColor(Color::Transparent);
		map[9][r].setFillColor(Color::Red);

		//Row 7
		r = 6;
		map[0][r].setFillColor(Color::Red);
		map[1][r].setFillColor(Color::Transparent);
		map[2][r].setFillColor(Color::Green);
		map[3][r].setFillColor(Color::Transparent);
		map[4][r].setFillColor(Color::Transparent);
		map[5][r].setFillColor(Color::Transparent);
		map[6][r].setFillColor(Color::Green);
		map[7][r].setFillColor(Color::Transparent);
		map[8][r].setFillColor(Color::Green);
		map[9][r].setFillColor(Color::Red);

		//Row 8
		r = 7;
		map[0][r].setFillColor(Color::Red);
		map[1][r].setFillColor(Color::Transparent);
		map[2][r].setFillColor(Color::Green);
		map[3][r].setFillColor(Color::Green);
		map[4][r].setFillColor(Color::Transparent);
		map[5][r].setFillColor(Color::Green);
		map[6][r].setFillColor(Color::Green);
		map[7][r].setFillColor(Color::Transparent);
		map[8][r].setFillColor(Color::Transparent);
		map[9][r].setFillColor(Color::Red);

		//Row 9
		r = 8;
		map[0][r].setFillColor(Color::Red);
		map[1][r].setFillColor(Color::Transparent);
		map[2][r].setFillColor(Color::Transparent);
		map[3][r].setFillColor(Color::Transparent);
		map[4][r].setFillColor(Color::Transparent);
		map[5][r].setFillColor(Color::Transparent);
		map[6][r].setFillColor(Color::Transparent);
		map[7][r].setFillColor(Color::Transparent);
		map[8][r].setFillColor(Color::Green);
		map[9][r].setFillColor(Color::Red);

		//Row 9
		r = 9;
		map[0][r].setFillColor(Color::Red);
		map[1][r].setFillColor(Color::Red);
		map[2][r].setFillColor(Color::Red);
		map[3][r].setFillColor(Color::Red);
		map[4][r].setFillColor(Color::Red);
		map[5][r].setFillColor(Color::Red);
		map[6][r].setFillColor(Color::Red);
		map[7][r].setFillColor(Color::Red);
		map[8][r].setFillColor(Color::Red);
		map[9][r].setFillColor(Color::Red);
	}

};