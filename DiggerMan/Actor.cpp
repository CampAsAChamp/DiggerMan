#include <iostream>
#include "Actor.h"
#include "StudentWorld.h"
#include <algorithm>

using namespace std;

const int exitPointX = 60;
const int exitPointY = 60;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
void Actor::doSomething()
{}

void DiggerMan::doSomething()
{
    //cout << "X: " << getX() << "| Y: " << getY() << endl;
    StudentWorld* world = getWorld();
    //    cout << "DIRECTION: " << getDirection() << endl;

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
    StudentWorld* world = getWorld();
    return world->checkDirtBelow(getX(), getY());
}

void Boulder::doSomething()
{
    if (!isAlive())
    {
        return;
    }

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
            //Boulder is now stable at the bottom on top of some dirt
        }

        else if (getWorld()->checkBoulderBelow(getX(), getY()))
        {
            m_state = stable;
            setHitpoints(0);
        }

        else if (getWorld()->checkDiggerman(getX(), getY(), down))
        {
            m_state = stable;
            setHitpoints(0);
            getWorld()->setDiggermanHP(0);
            //Boulder is on top of diggerman
        }

        else
        {
            moveTo(getX(), (getY() - 1)); //If there isn't any dirt below it and not at the bottom, then keep falling
        }
    }
}

void WaterPool::doSomething()
{
    StudentWorld* world = getWorld();
    DiggerMan* diggerMan = world->getDiggerMan();

    int level = world->getLevel();


    //IMPLEMENTED 5/25/17 1:06 AM JOSEPH TICKS TO MAKE WATER POOL DISAPPEAR
    int t = max(100, 300 - 10 * level);

    if (world->getTicks() == t)
    {
        setHitpoints(0);
        return;
    }

    //    cout << "DIGGERMAN W LOC: " << diggerMan->getX() << endl;
    //    cout << "DIGGERMAN W LOC: " << diggerMan->getY() << endl;
    //    cout << "WATERPOOL LOC: " << getX() << endl;
    //    cout << "WATERPOOL LOC: " << getY() << endl;


    if (world->checkDiggerman(getX(), getY(), getDirection()))
    {
        setHitpoints(0);
        world->playSound(SOUND_GOT_GOODIE);
        diggerMan->addWater(5);
        world->increaseScore(100);

    }
}

void Barrel::doSomething()
{
    StudentWorld* world = getWorld();

    if (!isAlive())
    {
        return;
    }

    if (this->isVisible() && world->checkDiggerman(this->getX(), this->getY(), this->getDirection()))
    {
        cout << "GOT OIL" << endl;
        setHitpoints(0);
        world->playSound(SOUND_FOUND_OIL);
        world->increaseScore(1000);
    }
}

//PROTESTER 
void Protester::doSomething()
{
    if (!isAlive())
        return;

    if (waitingTime < tickToWaitBetweenMoves)
    {
        waitingTime++;
        cout << "Waiting for " << waitingTime << " ticks \n";
        return;
    }

    if (m_state == leaveOilField)
    {
        if (getX() == exitPointX && getY() == exitPointY) //if at the exit point
        {
            setHitpoints(0);
            return;
        }
    }

    else if (waitingTime >= tickToWaitBetweenMoves && getWorld()->checkDiggerman(getX(),getY(), down) && nonRestingTicks >= 50 && getWorld()->protesterFacingDiggerman(getX(),getY(),getDirection())) 
        //TODO: Check if 4 units from diggerman
        //Might want to change the checkDiggerman function depending on what unit actually means
    {
        waitingTime = 0;
        nonRestingTicks = 0;
        getWorld()->annoyDiggerman(2); //Cause 2 points of annoyance to diggerman
        getWorld()->playSound(SOUND_PROTESTER_YELL);
        cout << "Protester yelled at Diggerman\n";
        return;
    }

    else if (getWorld()->getDiggerMan()->getX() == this->getX() || getWorld()->getDiggerMan()->getY() == this->getY()) //Check to see if diggerman is in a straight line from protester


    nonRestingTicks++;
}

//SQUIRT // GOTTA IMPLEMENT THE REST WHEN PROTESTER IS IMPLEMENTED
void Squirt::doSomething() //BOTTOM OF MAP ERROR FIX LATER
{
    if (!isAlive())
    {
        return;
    }

    StudentWorld* world = getWorld();
    DiggerMan* diggerMan = world->getDiggerMan();


    cout << "Water: " << diggerMan->getWater() << endl;


    if (distanceTraveled == 3)
    {
        setHitpoints(0);
    }

    if (world->checkProtester(getX(), getY(), right))
    {
        world->annoyProtester(1);
    }




    //SQUIRT MOVEMENT
    switch (getDirection())
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
