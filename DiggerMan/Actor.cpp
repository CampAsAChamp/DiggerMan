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
        if (waitTime == 3) //MAKES SURE THEY CANT SPAM SPACEBAR
        {
            if (numValue == KEY_PRESS_SPACE)
            {
                world->squirt(getX(), getY(), getDirection());
                world->playSound(SOUND_PLAYER_SQUIRT);
                waitTime = 0;
            }
        }
        //For Sonar sound
        if (numValue == 'z' && getSonar()>0)
        {
            decreaseSonar();
            world->playSound(SOUND_SONAR);

            for (int i = 1; i < 13; i++)
            {
                world->checkItems(getX() + i, getY());
                world->checkItems(getX() - i, getY());
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
        //		cout << "\tBoulder at " << getX() << "|" << getY() << " is dead\n";
        return;
    }

    if (m_state != waiting && m_state != falling) //If boulder is in normal state
    {
        if (isStable())
        {
            m_state = stable;
            //			cout << "\tBoulder at " << getX() << "|" << getY() << " is stable\n";
        }
        else
        {
            m_state = waiting;
            //			cout << "\tBoulder at " << getX() << "|" << getY() << " is waiting\n";
        }
        //		cout << "-----------------------------------------\n";
    }
    else if (waitTime < 30 && m_state == waiting)
    {
        waitTime++;
        //		cout << "\tBoulder at " << getX() << "|" << getY() << " is waiting for " << waitTime << " ticks\n";
        return;
    }
    else if (waitTime >= 30 && m_state == waiting)
    {
        waitTime = 0;
        m_state = falling;
        //		cout << "\tBoulder at " << getX() << "|" << getY() << " is moving to a falling state\n";
        getWorld()->playSound(SOUND_FALLING_ROCK);

    }
    else if (m_state == falling)
    {
        //Move down one square each tick until it hits the bottom of the field
        //Runs on top of another boulder
        //Runs into dirt (By moving down a square the boulder would overlap
        //Then must set state to dead so it can be removed from game
        //		cout << "\tBoulder at " << getX() << "|" << getY() << " is falling\n";

        if (getWorld()->checkDirtBelow(getX(), getY()) || getY() < 1)
        {
            m_state = stable;
            setHitpoints(0);
            //			cout << "\tBoulder at " << getX() << "|" << getY() << " is dead at the bottom\n";
            //Boulder is now stable at the bottom and waits to get cleared at the end of the current tick
        }

        else if (getWorld()->checkBoulderBelow(getX(), getY()))
        {
            m_state = stable;
            setHitpoints(0);
            //			cout << "\tBoulder at " << getX() << "|" << getY() << " hit another boulder\n";
        }

        else if (getWorld()->checkDiggermanBelow(getX(), getY())) //TODO: Fix radius of DiggerMan check
        {
            m_state = stable;
            getWorld()->setDiggermanHP(0);
            setHitpoints(0);
            //			cout << "\t Boulder hit DiggerMan\n";
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

    cout << "GAME TICKS: " << ticks << endl;
    cout << "WATERPOOL TICKS TO GO AWAY: " << t << endl;

    if (ticks == t)
    {
        setHitpoints(0);
        return;
    }

    if (world->checkDiggerman(getX(), getY(), getDirection()))
    {
        setHitpoints(0);
        world->playSound(SOUND_GOT_GOODIE);
        diggerMan->addWater(5);
        world->increaseScore(100);

    }

    ticks++;


}

void Barrel::doSomething()
{
    StudentWorld* world = getWorld();

    if (!isAlive())
    {
        return;
    }

    if (world->barrelVisible(this->getX(), this->getY()))
    {
        setVisible(true);
    }

    if (this->isVisible() && world->checkDiggerman(this->getX(), this->getY(), this->getDirection()))
    {
        cout << "GOT OIL" << endl;
        setHitpoints(0);
        world->playSound(SOUND_FOUND_OIL);
        world->increaseScore(1000);
        world->decreaseOil();
    }
}

//PROTESTER //IMPLEMENT NEXT

void Protester::doSomething()
{
    if (!isAlive() && m_state != leaveOilField)
    {
        setState(leaveOilField);
        return;
    }

    if (waitingTime < tickToWaitBetweenMoves)
    {
        waitingTime++;
        cout << "Waiting for " << waitingTime << " ticks \n";
        return;
    }

    tickToWaitBetweenMoves = std::max(0, (3 - (1 / 4)));

    //LEAVING OIL FIELD HANDLER////
    if (m_state == leaveOilField)
    {
        if (getY() <= 60 - 1)
        {
            moveTo(getX(), getY() + 1);
            setDirection(up);
            return;
        }

        if (getX() <= 60 - 1)
        {
            moveTo(getX() + 1, getY());
            setDirection(right);
            return;
        }

        if (getX() == 60 && getY() == 60)
        {
            setHitpoints(0);
            setState(dead);
        }

    }

    if (ticks < 7)
    {
        ticks++;
        return;
    }

    else if (waitingTime >= tickToWaitBetweenMoves && getWorld()->checkDiggerman(getX(), getY(), down) && nonRestingTicks >= 15 && getWorld()->protesterFacingDiggerman(getX(), getY(), getDirection()))
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

    int counterX = getX();
    int counterY = getY();

    for (int i = 0; i <= 4; i++)
    {
        if (getWorld()->getDiggerMan()->getX() == this->getX() || (getWorld()->getDiggerMan()->getY() == this->getY() && !getWorld()->checkDiggerman(counterX + i, counterY, down))) //Check to see if diggerman is in a straight line from protester
        {
            cout << "Diggerman in line of shight 4 \n";
        }
    }
    nonRestingTicks++;


    //PROTESTER GETTING A RANDOM DIRECTION
    int done = false;


    cout << "TICKS: " << ticks << endl;
    cout << "TICKS TO WAIT: " << tickToWaitBetweenMoves << endl;


    while (!done)
    {
        int randomNum = rand() % 4;

        switch (randomNum)
        {
            case 0:
                setDirection(up);
                done = true;
                ticks = 0;
                break;
            case 1:
                setDirection(down);
                done = true;
                ticks = 0;
                break;
            case 2:
                setDirection(left);
                done = true;
                ticks = 0;
                break;
            case 3:
                setDirection(right);
                done = true;
                ticks = 0;
                break;
            default:
                break;

        }
    }



    //PROTESTER MOVING
    if (ticks == 0)
    {
        if (getX() == MAXSIZE_X - 5)
        {
            moveTo(getX() + 1, getY());
            setDirection(left);
        }
        if (getX() == 0)
        {
            moveTo(getX() + 1, getY());
            setDirection(left);
        }
        if (getX() == 60)
        {
            moveTo(getX() - 1, getY());
            setDirection(left);
        }
        if (getY() == 60)
        {
            moveTo(getX(), getY() - 1);
            setDirection(down);
        }


        //MOVING

        if (getDirection() == right && getX() <= MAXSIZE_X - 5 && !getWorld()->checkDirt(getX(), getY()))
        {
            moveTo(getX() + 1, getY());
            step = false;
            return;
        }
        else
        {
            ticks++;
            step = true;
        }
        if (getDirection() == down  && getY() >= 1 && !getWorld()->checkDirt(getX(), getY()))
        {
            moveTo(getX(), getY() - 1);
            step = false;
            return;
        }
        else
        {
            ticks++;
            step = true;
        }
        if (getDirection() == up && getY() <= MAXSIZE_Y - 5 && !getWorld()->checkDirt(getX(), getY()))
        {
            moveTo(getX(), getY() + 1);
            step = false;
            return;
        }
        else
        {
            ticks++;
            step = true;
        }
        if (getDirection() == left && getX() >= 1 && !getWorld()->checkDirt(getX(), getY()))
        {
            moveTo(getX() - 1, getY());
            step = false;
            return;
        }
        else
        {
            ticks++;
            step = true;
        }
    }



    cout << "X: " << getX() << "Y: " << getY() << endl;


    ticks++;




}

//SQUIRT // GOTTA IMPLEMENT THE REST WHEN PROTESTER IS IMPLEMENTED
void Squirt::doSomething() //BOTTOM OF MAP ERROR FIX LATER
{
    if (!isAlive())
    {
        return;
    }


    StudentWorld * world = getWorld();


    if (distanceTraveled == 3)
    {
        setHitpoints(0);
    }

    if (world->checkProtester(getX(), getY(), right))
    {
        world->annoyProtester(1);
        setHitpoints(0);
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

void SonarKit::doSomething()
{
    StudentWorld* world = getWorld();
    DiggerMan* diggerman = world->getDiggerMan();

    if (!isAlive())
        return;

    int t = max(100, 300 - 10 * world->getLevel());

    if (ticks == t)
    {
        setHitpoints(0);
        return;
    }

    cout << "GAME TICKS: " << ticks << endl;
    cout << "SONAR TICKS TO GO AWAY: " << t << endl;

    if (this->isVisible() && world->checkDiggerman(this->getX(), this->getY(), this->getDirection()))
    {
        cout << "GOT SONAR KIT" << endl;
        setHitpoints(0);
        world->playSound(SOUND_GOT_GOODIE);
        world->increaseScore(75);
        diggerman->increaseSonar();
    }

    ticks++;
}

void GoldNugget::doSomething()
{
    StudentWorld* world = getWorld();
    DiggerMan* diggerman = world->getDiggerMan();
    Protester* protester = world->getProtester();

    if (!isAlive())
        return;

    if (!isVisible() && world->barrelVisible(this->getX(), this->getY()))
    {
        setVisible(true);
    }

    if (this->isVisible() && world->checkDiggerman(this->getX(), this->getY(), this->getDirection()))
    {
        cout << "GOT GOLD NUGGET" << endl;
        setHitpoints(0);
        world->playSound(SOUND_GOT_GOODIE);
        world->increaseScore(10);
        diggerman->increaseGold();
    }

    if (world->checkProtester(this->getX(), this->getY(), right))
    {
        cout << "PROTESTER GOT GOLD" << endl;
        setHitpoints(0);
        world->playSound(SOUND_PROTESTER_FOUND_GOLD);
        world->increaseScore(25);
        protester->setState(rest);
    }


}