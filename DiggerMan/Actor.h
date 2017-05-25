#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameWorld.h"
#include <algorithm>


const int DM_START_X = 30;
const int DM_START_Y = 60;

class StudentWorld; //Forward declaration

enum BoulderState
{
    stable, falling, waiting
};

enum ProtesterState
{
	rest, leaveOilField
};

class Actor : public GraphObject
{
public:
    Actor(StudentWorld * world, int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0)
        :GraphObject(imageID, startX, startY, right, size, depth)
    {
        m_hitpoints = 1;
        m_world = world;
    }

    inline void setHitpoints(int newHitpoints)
    {
        this->m_hitpoints = newHitpoints;
    }
    inline int getHitpoints()
    {
        return m_hitpoints;
    }
    virtual bool isAlive()
    {
        return m_hitpoints > 0;
    }
    virtual StudentWorld* getWorld()
    {
        return m_world;
    }

    virtual void doSomething();

private:
    int m_hitpoints;
    StudentWorld * m_world;
};

class DiggerMan : public Actor
{
public:
    DiggerMan(StudentWorld * world)
        : Actor(world, IMID_PLAYER, DM_START_X, DM_START_Y)
    {
        setVisible(true);
        m_water = 5;
    }

    virtual void doSomething();
    
    inline int getWater()
    
    {
        return m_water;
    }
    inline void setWater(int water)
    {
        m_water = water;
    }
    inline void reduceWater()
    {
        m_water--;
    }
    
    inline DiggerMan* getDiggerMan()
    {
        return this;
    }

    virtual StudentWorld* getWorld()
    {
        return Actor::getWorld();
    }

private:
    int m_water;
    int m_sonarCharges;
    int m_goldNuggets;
};

class Squirt : public Actor
{
public:
    
    Squirt(StudentWorld* world, int startX, int startY, Direction dir)
    : Actor(world, IMID_WATER_SPURT, startX, startY, dir, 1.0, 1)
    {
        setVisible(true);
        distanceTraveled = 0;
    }
    
    virtual void doSomething();
    
private:
    
    int distanceTraveled;
    
    
};

class Dirt : public Actor
{
public:
    Dirt(StudentWorld * world, int startX, int startY)
        : Actor(world, IMID_DIRT, startX, startY, right, 0.25, 3)
    {
        setVisible(true);
    }

private:

};

class Barrel : public Actor
{
public:
    Barrel(StudentWorld * world, int startX, int startY)
        : Actor(world, IMID_BARREL, startX, startY, right, 1.0, 2)
    {
        setVisible(false); //Barrels should start hidden and only be discovered when walked over
    }
};

class GoldNugget : public Actor
{
public:
    GoldNugget(StudentWorld * world, int startX, int startY)
        : Actor(world, IMID_GOLD, startX, startY, right, 1.0, 2)
    {
        setVisible(true);
    }
};

class Boulder : public Actor
{
    //Must start in a stable state, must add code later
public:
    Boulder(StudentWorld * world, int startX, int startY)
        :Actor(world, IMID_BOULDER, startX, startY, down, 1.0, 1)
    {
        this->m_state = stable;
        setVisible(true);
    }

    void doSomething();
	bool isStable();
    BoulderState getState()
    {
        return m_state;
    }

private:
    BoulderState m_state;
    unsigned int waitTime = 0;
};

class Protester : public Actor
{
public:

	Protester(StudentWorld * world, int startX, int startY)
		:Actor(world, IMID_PROTESTER, startX, startY, left, 1.0, 0)
	{
		setVisible(true);
		setHitpoints(5);
	}

	void doSomething();
	ProtesterState getState() { return m_state; }

private:
	unsigned int tickToWaitBetweenMoves = std::max(0, (3 - (1 / 4)));
	unsigned int waitingTime = 0;
	unsigned int nonRestingTicks = 0;

	ProtesterState m_state;

};

class HardcoreProtester : public Protester
{};

#endif // ACTOR_H_
