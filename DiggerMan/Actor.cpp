#include <iostream>
#include "Actor.h"
#include "StudentWorld.h"

using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
void Actor::doSomething()
{}

void DiggerMan::doSomething()
{
	cout << "X: " << getX() << "| Y: " << getY() << endl;
	StudentWorld* world = getWorld();
	
	int numValue = 0;
	
	//////////MOVE HANDLER//////////
	if (world->getKey(numValue))
	{
		if (numValue == KEY_PRESS_LEFT)
		{
			if (getDirection() != left)
			{
				setDirection(left);
			}

			else if (getX() >= 1 && getDirection() == left)
			{
				moveTo(getX() - 1, getY());
				world->deleteDirt(getX(), getY());
				setDirection(left);
			}
		}
		if (numValue == KEY_PRESS_UP)
		{
			if (getDirection() != up)
			{
				setDirection(up);
			}
			
			else if (getY() <= MAXSIZE_Y - 5 && getDirection() == up)
			{
				moveTo(getX(), getY() + 1);
				world->deleteDirt(getX(), getY());
				setDirection(up);
			}
		}
		if (numValue == KEY_PRESS_RIGHT)
		{
			if (getDirection() != right)
			{
				setDirection(right);
			}
			
			else if (getX() <= MAXSIZE_X - 1 && getDirection() == right)
			{
				moveTo(getX() + 1, getY());				
				world->deleteDirt(getX(), getY());
				setDirection(right);
			}
		}
		if (numValue == KEY_PRESS_DOWN)
		{
			if (getDirection() != down)
			{
				setDirection(down);
			}
			
			else if (getY() >= 1 && getDirection() == down)
			{
				moveTo(getX(), getY() - 1);
				world->deleteDirt(getX(), getY());
				setDirection(down);
			}
			
		}
	}
}

void Boulder::doSomething()
{
	//TODO: Check if still alive and immediately return
	if (getHitpoints() == 0)
		return;

	if (getState() == stable)
		return;
	else
	{
		//Push to waiting state
		m_state = waiting;

	}
}

bool Boulder::isStable()
{
	StudentWorld* world = getWorld();
	return world->checkDirt(getX(), getY());
}
