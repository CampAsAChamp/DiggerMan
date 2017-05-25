#include <iostream>
#include "Actor.h"
#include "StudentWorld.h"
#include <algorithm>

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
    
    if (waitTime < 3)
    {
        waitTime++;
        return;
    }

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
        if (waitTime == 3)
        {
            if (numValue == KEY_PRESS_SPACE)
            {
                world->squirt(getX(), getY(), getDirection());
                world->playSound(SOUND_PLAYER_SQUIRT);
                waitTime = 0;
            }
        }
    }
}

bool Boulder::isStable()
{
	return getWorld()->checkDirtBelow(getX(), getY());
}

void Boulder::doSomething()
{
	if (!isAlive())
		return;

	if (m_state != waiting && m_state != falling) //If boulder is in normal state
	{
		if (isStable())
		{
			m_state = stable;
		}
		else
		{
			m_state = waiting;
		}
	}
	else if (waitTime < 30 && m_state == waiting)
	{
		waitTime++;
		return;
	}
	else if (waitTime >= 30 && m_state == waiting)
	{
		waitTime = 0;
		m_state = falling;
		getWorld()->playSound(SOUND_FALLING_ROCK);

	}
	else if (m_state == falling)
	{
		if (getWorld()->checkDirtBelow(getX(), getY()) || getY() < 1)
		{
			m_state = stable;
			setHitpoints(0);
		}

		else if (getWorld()->checkBoulderBelow(getX(), getY()))
		{
			m_state = stable;
			setHitpoints(0);
		}

		else if (getWorld()->checkDiggermanBelow(getX(), getY())) //TODO: Fix radius of DiggerMan check
		{
			m_state = stable;
			getWorld()->setDiggermanHP(0);
			cout << "\t Boulder hit DiggerMan\n";
		}

		else
		{
			moveTo(getX(), (getY() - 1)); //If there isn't any dirt below it and not at the bottom, then keep falling
		}
	}
}

void Squirt::doSomething() //BOTTOM OF MAP ERROR FIX LATER
  
void WaterPool::doSomething()
{
    StudentWorld* world = getWorld();
    DiggerMan* diggerMan = world->getDiggerMan();
    

    
    cout << "DIGGERMAN W LOC: " << diggerMan->getX() << endl;
    cout << "DIGGERMAN W LOC: " << diggerMan->getY() << endl;
    cout << "WATERPOOL LOC: " << getX() << endl;
    cout << "WATERPOOL LOC: " << getY() << endl;
    
    if (diggerMan->getX() == getX() && diggerMan->getY() == getY())
    {
        setHitpoints(0);
        world->playSound(SOUND_GOT_GOODIE);
        diggerMan->addWater(5);
        world->increaseScore(100);
        
    }
}

//PROTESTER //IMPLEMENT NEXT
void Protester::doSomething()
{
	StudentWorld* world = getWorld();
	DiggerMan* diggerMan = world->getDiggerMan();

	cout << "Water: " << diggerMan->getWater() << endl;

	if (distanceTraveled == 3)
	{
		setHitpoints(0);
	}

	switch (getDirection())
	{
		case up:
			if (world->checkDirtOrActor(getX(), getY() + 1) == false)
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
			if (world->checkDirtOrActor(getX(), getY() - 1) == false)
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
			if (world->checkDirtOrActor(getX() - 1, getY()) == false)
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
			if (world->checkDirtOrActor(getX() + 1, getY()) == false)
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

void Protester::doSomething()
{
	if (!isAlive())
		return;

	if (waitingTime < tickToWaitBetweenMoves)
	{
		waitingTime++;
		cout << "Waiting for " << waitingTime << "ticks \n";
	}

	else if (waitingTime >= tickToWaitBetweenMoves && getWorld()->checkDiggerman(getX(), getY(), getDirection()) && nonRestingTicks > 15) //Check if 4 units away from DiggerMan
	{
		waitingTime = 0;
		nonRestingTicks = 0;
		getWorld()->annoyDiggerman(2); //Cause 2 points of annoyance to diggerman
		getWorld()->playSound(SOUND_PROTESTER_YELL);
		cout << "Protester yelled at Diggerman";
	}

	

}

