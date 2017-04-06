#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir)
    : GameWorld(assetDir)
    {
    }
    
    virtual int init()
    {
        for (int i = 0; i < 60; i++) //VERTICAL AXIS
        {
            for (int j = 0; j < 65; j++) //HORIZONTAL AXIS
            {
                if (i < 9)
                {
                    dirt[i][j] = new Dirt(j, i);
                }
                else
                {
                    if (j <= 29 || j >= 34)
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
        
        int numValue;
        
        
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
            else if (numValue == right)
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
    {
    }
    
private:
    
    Actor* m_actor[60][60];
    DiggerMan* diggerman[60][60];
    Dirt* dirt[60][60];
    
};

#endif // STUDENTWORLD_H_
