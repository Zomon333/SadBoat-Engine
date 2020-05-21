#pragma once
#include "headers.h"
using namespace std;

class Polygon //This used to be named Shape but SFML is a pain in the ass
{
private:
	int lineCount;
	Line* bounds[];
public:
	Line* getBounds();
	void setBounds(Line nBounds[]);

	int getLineCount();
	void setLineCount(int nLineCount);

	Polygon(Line nBounds[]);
	//bool collidingWith(Polygon toCheck);
};

