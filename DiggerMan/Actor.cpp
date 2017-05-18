#include <iostream>
#include "Actor.h"
#include "StudentWorld.h"

using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
void Actor::doSomething()
{}

void DiggerMan::doSomething()
{
    //cout << "X: " << getX() << "| Y: " << getY() << endl;
    StudentWorld* world = getWorld();
    cout << "DIRECTION: " << getDirection() << endl;

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

            else if (getX() <= MAXSIZE_X - 5 && getDirection() == right)
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
        if (numValue == KEY_PRESS_SPACE)
        {
            world->squirt(getX(), getY(), getDirection());
            world->playSound(SOUND_PLAYER_SQUIRT);
        }
    }
}

bool Boulder::isStable()
{
    StudentWorld* world = getWorld();
    //return world->checkDirtBelow(getX(), getY());
    return world->checkActorBelow(getX(), getY(),IMID_DIRT);
}

void Boulder::doSomething()
{
    //TODO: Check if still alive and immediately return
    if (!isAlive())
    {
        cout << "\tBoulder at " << getX() << "|" << getY() << " is dead\n";
        return;
    }

    if (m_state != waiting && m_state != falling) //If boulder is in normal state
    {
        if (isStable())
        {
            m_state = stable;
            cout << "\tBoulder at " << getX() << "|" << getY() << " is stable\n";
        }
        else
        {
            m_state = waiting;
            cout << "\tBoulder at " << getX() << "|" << getY() << " is waiting\n";
        }
        cout << "-----------------------------------------\n";
    }
    else if (waitTime < 30 && m_state == waiting)
    {
        waitTime++;
        cout << "\tBoulder at " << getX() << "|" << getY() << " is waiting for " << waitTime << " ticks\n";
        return;
    }
    else if (waitTime >= 30 && m_state == waiting)
    {
        waitTime = 0;
        m_state = falling;
        cout << "\tBoulder at " << getX() << "|" << getY() << " is moving to a falling state\n";
        getWorld()->playSound(SOUND_FALLING_ROCK);

    }
    if (m_state == falling)
    {
        //Move down one square each tick until it hits the bottom of the field
        //Runs on top of another boulder
        //Runs into dirt (By moving down a square the boulder would overlap
        //Then must set state to dead so it can be removed from game
        cout << "\tBoulder at " << getX() << "|" << getY() << " is falling\n";

		if (getY() >= 1 && !getWorld()->checkActorBelow(getX(), getY(), IMID_DIRT)) //If there isn't any dirt below it and not at the bottom, then keep falling
        {
            moveTo(getX(), (getY() - 1));
        }
		else if (getWorld()->checkActorBelow(getX(), getY(), IMID_BOULDER))
		{
			m_state = stable;
			setHitpoints(0);
			cout << "\tBoulder at " << getX() << "|" << getY() << " hit another boulder\n";
		}
        else 
        {
			//Boulder is now stable at the bottom and waits to get cleared at the end of the current tick
            m_state = stable;
            setHitpoints(0);
            cout << "\tBoulder at " << getX() << "|" << getY() << " is dead\n";
        }


    }
}
//PROTESTER //IMPLEMENT NEXT
void Protester::doSomething()
{
    
    
    StudentWorld* world = getWorld();
    int ticksToWaitBetweenMoves = max(0, (3 - (1/4)));
    
    
}

//SQUIRT // GOTTA IMPLEMENT THE REST WHEN PROTESTER IS IMPLEMENTED
void Squirt::doSomething() //BOTTOM OF MAP ERROR FIX LATER
{
    StudentWorld* world = getWorld();
    DiggerMan* diggerMan = world->getDiggerMan();

    
    cout << "Water: " << diggerMan->getWater() << endl;
    
    if (distanceTraveled == 3)
    {
        setHitpoints(0);
    }
    
    switch(getDirection())
    {
        case up:
            if (world->checkDirt(getX(), getY() + 1) == false)
            {
                moveTo(getX(), getY() + 1);
                distanceTraveled++;
            }
            else
            {
                setHitpoints(0);
            }
            
            return;
        case down:
            if (world->checkDirt(getX(), getY() - 1) == false)
            {
                moveTo(getX(), getY() - 1);
                distanceTraveled++;
            }
            else
            {
                setHitpoints(0);
            }
            
            return;
        case left:
            if (world->checkDirt(getX() - 1, getY()) == false)
            {
                moveTo(getX() - 1, getY());
                distanceTraveled++;
            }
            else
            {
                setHitpoints(0);
            }
            
            return;
            
        case right: 
            if (world->checkDirt(getX() + 1, getY()) == false)
            {
                moveTo(getX() + 1, getY());
                distanceTraveled++;
            }
            else
            {
                setHitpoints(0);
            }
            
            return;
            
        default: setHitpoints(0);
            return;
    }

}
