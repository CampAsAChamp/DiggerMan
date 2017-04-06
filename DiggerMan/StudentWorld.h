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
                    m_actor[i] = new Dirt(j, i);
                }
                else
                {
                    if (j <= 29 || j >= 34)
                    {
                        
                        
                        m_actor[i] = new Dirt(j, i);
                    }
                }
            }
        }
        diggerman[0] = new DiggerMan();
        
        return GWSTATUS_CONTINUE_GAME;
    }
    
    virtual int move()
    {
        // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
        // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
        decLives();
        return GWSTATUS_CONTINUE_GAME;
    }
    
    virtual void cleanUp()
    {
    }
    
private:
    
    Actor* m_actor[60];
    Actor* diggerman[1];
    
};

#endif // STUDENTWORLD_H_
