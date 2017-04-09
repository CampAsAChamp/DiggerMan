#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>

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
                    dirt[i][j] = new Dirt(j, i); //Add dirt to below the mineshaft
                }
                else
                {
                    if (j <= MINESHAFT_START_LEFT || j >= MINESHAFT_STOP_RIGHT) //If left or right to the mineshaft
                    {
                        dirt[i][j] = new Dirt(j, i);
                    }
                }
            }
        }

        diggerman[0][0] = new DiggerMan();
        
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
        
		if ((diggerman[0][0]->getX() + 1) > MAXSIZE_X)
        {
            outOfBounds = true;
        }
        
        if (getKey(numValue))
        {
            if (numValue == up)
            {
                diggerman[0][0]->doSomething(KEY_PRESS_UP);
            }
            else if (numValue == down)
            {
                diggerman[0][0]->doSomething(KEY_PRESS_DOWN);
            }
            else if (numValue == right && outOfBounds == false)
            {
                diggerman[0][0]->doSomething(KEY_PRESS_RIGHT);
            }
            else if (numValue == left)
            {
                diggerman[0][0]->doSomething(KEY_PRESS_LEFT);
            }
  
        }
  
        return GWSTATUS_CONTINUE_GAME;
    }
    
    virtual void cleanUp()
    {}
    
private:
    
	Actor* m_actor[MAXSIZE_X][MAXSIZE_Y];
	DiggerMan* diggerman[MAXSIZE_X][MAXSIZE_Y];
	Dirt* dirt[MAXSIZE_X][MAXSIZE_Y];
    
};

#endif // STUDENTWORLD_H_
