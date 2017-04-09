#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>

//This is Nick's branch
const int MAXSIZE_X = 60;
const int MAXSIZE_Y = 60;

const int MINESHAFT_START_LEFT = 29;
const int MINESHAFT_STOP_RIGHT = 34;
const int MINESHAFT_BOTTOM = 9;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir)
    : GameWorld(assetDir)
    {}
    
    virtual int init()
    {
		for (int i = 0; i < MAXSIZE_X; i++) //VERTICAL AXIS (X-Axis)
        {
			for (int j = 0; j < (MAXSIZE_Y +5); j++) //HORIZONTAL AXIS (Y-Axis)
            {
                if (i < MINESHAFT_BOTTOM) //If below the mineshaft
                {
                    m_actor[i][j] = new Dirt(j, i); //Add dirt to below the mineshaft
                }
                else
                {
                    if (j <= MINESHAFT_START_LEFT || j >= MINESHAFT_STOP_RIGHT) //If left or right to the mineshaft
                    {
                        m_actor[i][j] = new Dirt(j, i);
                    }
                }
            }
        }
		m_actor[20][20] = new Boulder(20, 20); //Testing to see if I can spawn a boulder
		m_actor[30][30] = new Barrel(30, 30); //Testing to see if I can spawn a barrel
		m_actor[40][40] = new GoldNugget(40, 40);
		m_diggerman = new DiggerMan();
        
        return GWSTATUS_CONTINUE_GAME;
    }
    
    virtual int move()
    {
        // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
        // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
        
        int up = KEY_PRESS_UP;
        int down = KEY_PRESS_DOWN;
        int left = KEY_PRESS_LEFT;
        int right = KEY_PRESS_RIGHT;
        
        int numValue = 0;
        bool outOfBounds = false;
        
		if ((m_diggerman->getX() + 1) > MAXSIZE_X)
        {
            outOfBounds = true;
        }
        
        if (getKey(numValue))
        {
            if (numValue == up)
            {
                m_diggerman->doSomething(KEY_PRESS_UP);
            }
            else if (numValue == down)
            {
                m_diggerman->doSomething(KEY_PRESS_DOWN);
            }
            else if (numValue == right && outOfBounds == false)
            {
                m_diggerman->doSomething(KEY_PRESS_RIGHT);
            }
            else if (numValue == left)
            {
                m_diggerman->doSomething(KEY_PRESS_LEFT);
            }
        }
        return GWSTATUS_CONTINUE_GAME;
    }
    
    virtual void cleanUp()
    {}
    
private:
	Actor* m_actor[MAXSIZE_X][MAXSIZE_Y];
	DiggerMan* m_diggerman;

};

#endif // STUDENTWORLD_H_
