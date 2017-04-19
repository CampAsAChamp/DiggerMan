#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

void DiggerMan::doSomething(int value)
{
    //////////MOVE HANDLER//////////
    if (value == KEY_PRESS_LEFT)
    {
        moveTo(getX() - 1, getY());
        setDirection(left);
    }
    else if (value == KEY_PRESS_RIGHT)
    {
        moveTo(getX() + 1, getY());
        setDirection(right);
    }
    else if (value == KEY_PRESS_UP)
    {
        moveTo(getX(), getY() + 1);
        setDirection(up);
    }
    else if (value == KEY_PRESS_DOWN)
    {
        moveTo(getX(), getY() - 1);
        setDirection(down);
    }

    //////////BOUNDARIES HANDLER//////////
    bool outOfBounds = false;
    
    if ((getX() + 1) > 60)
    {
        outOfBounds = true;
        moveTo(getX() - 1, getY());
    }
    else if ((getX() - 1) < 0)
    {
        outOfBounds = true;
        moveTo(getX() + 1, getY());
    }
    else if ((getY() - 1) < 0)
    {
        outOfBounds = true;
        moveTo(getX(), getY() + 1);
    }
    else if ((getY() + 1) > 61)
    {
        outOfBounds = true;
        moveTo(getX(), getY() - 1);
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
	return true;
}