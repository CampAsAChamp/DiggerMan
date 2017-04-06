#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

void DiggerMan::doSomething(int value)
{
    bool outOfBounds = false;
    
    if ((getX() + 1) > 60)
    {
        outOfBounds = true;
    }
    else if ((getX() - 1) < 0)
    {
        outOfBounds = true;
    }

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
    
    if (outOfBounds == true)
    {
        
    }
}
