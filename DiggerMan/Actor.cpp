#include <iostream>
#include "Actor.h"
#include "StudentWorld.h"

using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

void DiggerMan::doSomething(int value)
{
	cout << "X: " << getX() << " | Y: " << getY() << endl;

	//////////MOVE HANDLER//////////
	if (value == KEY_PRESS_LEFT)
	{
		if (getDirection() != left)
		{
			setDirection(left);
		}

		else if (getX() >= 1 && getDirection() == left)
		{
			moveTo(getX() - 1, getY());
			setDirection(left);
		}
	}

	else if (value == KEY_PRESS_UP)
	{
		if (getDirection() != up)
		{
			setDirection(up);
		}

		else if (getY() <= MAXSIZE_Y - 5 && getDirection() == up)
		{
			moveTo(getX(), getY() + 1);
			setDirection(up);
		}
	}

	else if (value == KEY_PRESS_DOWN)
	{
		if (getDirection() != down)
		{
			setDirection(down);
		}

		else if (getY() >= 1 && getDirection() == down)
		{
			moveTo(getX(), getY() - 1);
			setDirection(down);
		}
	}

	else if (value == KEY_PRESS_RIGHT)
	{
		if (getDirection() != right)
		{
			setDirection(right);
		}

		else if (getX() <= MAXSIZE_X - 1 && getDirection() == right)
		{
			moveTo(getX() + 1, getY());
			setDirection(right);
		}
	}
}

void Boulder::doSomething()
{
	//TODO: Check if still alive and immediately return

}

bool Boulder::isStable()
{
	//TODO: Check if 4 dirt below
	//GraphObject G1 = getGraphObjects(getY());
	//GraphObject temp = getGraphObjects(getY());


	return true;
}
