#pragma once
#include "headers.h"
using namespace std;
using namespace sf;

float radToDegree(float radians)
{
	return (radians * 57.2958);
}

float degreeToRad(float degrees)
{
	return (degrees / 57.2958);
}

float FPSRaycast(Point origin, float angle, string direction, Line *toCheck[40][10], colormap map)
{
	const int HYPOTENUSE = 10000;
	Line raycast;
	Point start = origin;
	Point end;
	float dX, dY;
	if (direction != "LEFT" || direction != "RIGHT")
	{
		return 0;
	}

	if (angle < 1.57079633 - TOLERANCE || angle>1.57079633 + TOLERANCE)
		if (angle < 4.71238898 - TOLERANCE || angle>4.71238898 + TOLERANCE) // Is invalid for tangent
			dY = (HYPOTENUSE * cos(angle) * tan(angle));
		else
			dY = HYPOTENUSE;

	if (angle < 1.57079633 - TOLERANCE || angle>1.57079633 + TOLERANCE)
		if (angle < 4.71238898 - TOLERANCE || angle>4.71238898 + TOLERANCE) // Is invalid for tangent
			dX = (sin(angle) / (4 * tan(angle)));
		else
			dX = HYPOTENUSE;

	if (direction == "RIGHT")
	{
		end.setX(start.getX() + dX);
		end.setY(start.getY() + dY);
	}
	else
	{
		end.setX(start.getX() - dX);
		end.setY(start.getY() - dY);
	}


	raycast = Line(start, end);
	float shortestDistance = 100000000;

	int i = 39;
	int j = 9;
	while (i > -1 && j > -1)
	{
		if (raycast.checkIntersect(*toCheck[i][j], TOLERANCE))
		{
			Point intersect = raycast.whereIntersect(*toCheck[i][j]);
			if (raycast.getStart().getDistance(intersect) < shortestDistance && map.map[((i - (i % 4)) / 4)][j].getFillColor() != Color::Transparent)
			{
				shortestDistance = raycast.getStart().getDistance(intersect);
			}
		}

		i--;
		if (i == -1)
		{
			i = 39;
			j--;
		}

	}

	return shortestDistance;

}

RectangleShape FPSRaycastWhich(Point origin, float angle, string direction, Line *toCheck[40][10], colormap map)
{
	const int HYPOTENUSE = 10000;
	Line raycast;
	Point start = origin;
	Point end;
	float dX, dY;
	if (direction != "LEFT" || direction != "RIGHT")
	{
		//return 0;
	}

	if (angle < 1.57079633 - TOLERANCE || angle>1.57079633 + TOLERANCE)
		if (angle < 4.71238898 - TOLERANCE || angle>4.71238898 + TOLERANCE) // Is invalid for tangent
			dY = (HYPOTENUSE * cos(angle) * tan(angle));
		else
			dY = HYPOTENUSE;

	if (angle < 1.57079633 - TOLERANCE || angle>1.57079633 + TOLERANCE)
		if (angle < 4.71238898 - TOLERANCE || angle>4.71238898 + TOLERANCE) // Is invalid for tangent
			dX = (sin(angle) / (4 * tan(angle)));
		else
			dX = HYPOTENUSE;

	if (direction == "RIGHT")
	{
		end.setX(start.getX() + dX);
		end.setY(start.getY() + dY);
	}
	else
	{
		end.setX(start.getX() - dX);
		end.setY(start.getY() - dY);
	}


	raycast = Line(start, end);
	float shortestDistance = 100000000;

	int i = 39;
	int j = 9;
	while (i > -1 && j > -1)
	{
		if (raycast.checkIntersect(*toCheck[i][j], TOLERANCE))
		{
			Point intersect = raycast.whereIntersect(*toCheck[i][j]);
			if (raycast.getStart().getDistance(intersect) < shortestDistance)
			{
				shortestDistance = raycast.getStart().getDistance(intersect);
			}
		}

		i--;
		if (i == -1)
		{
			i = 39;
			j--;
		}

	}
	i = 39;
	j = 9;
	int k, l;
	while (i > -1 && j > -1)
	{

		if (i == -1)
		{
			i = 39;
			j--;
		}
	}
	return map.map[((i - (i % 4)) / 4)][j];
	//return shortestDistance;

}

int main()
{
	
	const int WIDTH = 1920;
	const int HEIGHT = 1080;
	const int resolution = 100;
	const int HORIZOFFSET = WIDTH / resolution;
	const int VERTOFFSET = HEIGHT / resolution;

	Point Player = Point(WIDTH/2 , HEIGHT/2);
	float angle = 0;

	RenderWindow window(VideoMode(WIDTH, HEIGHT), "Stonks");
	sf::Keyboard keyboard;
	keyboard.setVirtualKeyboardVisible(true);
	
	Vector2f rightCorner = Vector2f(100, 100);
	RectangleShape shape = RectangleShape(rightCorner);
	shape.setFillColor(Color::Green);
	shape.move(50, 50);

	class lazyPointGroup
	{
	private:
		Point A, B, C, D;
	public:
		Line AB, BC, CD, DA;
		Line bounds[4];
		Polygon lazySquare = Polygon(bounds);

		lazyPointGroup()
		{
			setOffset(0, 0);
		}

		void setOffset(float offsetX, float offsetY)
		{
			A = Point(0 + offsetX, 0 + offsetY);
			B = Point(4 + offsetX, 0 + offsetY);
			C = Point(4 + offsetX, 4 + offsetY);
			D = Point(0 + offsetX, 4 + offsetY);

			AB = Line(A, B);
			BC = Line(B, C);
			CD = Line(C, D);
			DA = Line(D, A);

			bounds[0] = AB;
			bounds[1] = BC;
			bounds[2] = CD;
			bounds[3] = DA;

			lazySquare.setBounds(bounds);

		}
	};

	Line* physics[40][10];
	lazyPointGroup *workingSquare;

	int i = 0;
	int j = 0;
	int workingIndex = 0;

	while (i < 10 && j < 10)
	{
		workingSquare = new lazyPointGroup();
		workingSquare->setOffset((i * 4), (j * 4));
		workingIndex = i * 4;

		physics[workingIndex][j] = &workingSquare->bounds[0];
		physics[workingIndex+1][j] = &workingSquare->bounds[1];
		physics[workingIndex+2][j] = &workingSquare->bounds[2];
		physics[workingIndex+3][j] = &workingSquare->bounds[3];

		i++;
		if (i == 10)
		{
			i = 0;
			j++;
		}
		if (j == 10)
		{
			break;
		}
	}

	
	

	colormap map = colormap();
	Line bounds[4];
	RectangleShape viewport[100];
	float distance = 10000000000;
	float distanceChart[resolution];
	while (window.isOpen())
	{
		distance = 10000000000;
		i = 0;
		workingIndex = 0;
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		
		while (i < 100)
		{
			distanceChart[i] = FPSRaycast(Player, (((i / 100) * 90) / 57.2958), "LEFT", physics ,map);
			//Raycast Theta for distance

			viewport[i] = FPSRaycastWhich(Player, (((i / 100) * 90) / 57.2958), "LEFT", physics, map);
			//Raycast Theta for rectangle

			viewport[i].setSize(Vector2f(HORIZOFFSET, ((distance / 40)* HEIGHT)));
			//Scale rectangle by distance

			viewport[i].move((HORIZOFFSET* i), ((HEIGHT - viewport[i].getSize().y) / 2));
			//Move rectangle by raycast section

			i++;
		}

		window.clear();
		window.draw(shape);
		window.display();
	}
	return 1;
}