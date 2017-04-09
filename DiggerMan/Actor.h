#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameWorld.h"

const int DM_START_X = 30;
const int DM_START_Y = 60;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

//GraphObject(int imageID, int startX, int startY, DIRECTION startDirection, float size = 1.0, unsigned int depth = 0);

class Actor : public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0)
    : GraphObject(imageID, startX, startY, right, size, depth)
    {}
    
    inline void setHitpoints(int newHitpoints)
    {
        this->m_hitpoints = newHitpoints;
    }
    inline int getHitpoints()
    {
        return m_hitpoints;
    }

    
private:
    int m_hitpoints;
};

class DiggerMan : public Actor
{
public:
    DiggerMan()
    : Actor(IMID_PLAYER, DM_START_X, DM_START_Y)
    {
        setVisible(true);
    }
    
    virtual void doSomething(const int value);
    
private:
    int m_water;
    int m_sonarCharges;
    int m_goldNuggets;
};

class Dirt : public Actor
{
public:
    Dirt(int startX, int startY)
    : Actor(IMID_DIRT, startX, startY, right, 0.25, 3)
    {
        setVisible(true);
    }
    
private:
    
};

class Boulder : public Actor
{
	//Must start in a stable state, must add code later
public:
	Boulder(int startX, int startY)
		:Actor(IMID_BOULDER, startX, startY, down, 1.0, 1)
	{
		setVisible(true);
	}

	void doSomething();
	bool isStable();

private:
};

class Protester : public Actor
{};

class HardcoreProtestor : public Protester
{};

#endif // ACTOR_H_
