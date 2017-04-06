#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

const int DM_POS_X = 30;
const int DM_POS_Y = 60;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

//GraphObject(int imageID, int startX, int startY, DIRECTION startDirection, float size = 1.0, unsigned int depth = 0);

class Actor: public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0)
    : GraphObject(imageID, startX, startY, right, size, depth) {}
    
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

class FixedActor : public Actor
{
    
};

class DiggerMan: public Actor
{
public:
    DiggerMan()
    : Actor(IMID_PLAYER, 30, 60)
    {
        setVisible(true);
    }
private:
    int m_water;
    int m_sonarCharges;
    int m_goldNuggets;
    
};

class Protestor : public Actor
{
    
};

class HardcoreProtestor : public Actor
{
    
};

class OilBarrel : public FixedActor
{
    
};

class Boulder : public FixedActor
{
    
};

class GoldNugget : public FixedActor
{
    
};

class SonarKit : public FixedActor
{
    
};

class WaterPickup : public FixedActor
{
    
};

class Dirt : public Actor
{
public:
    Dirt(int startX, int startY)
    : Actor(IMID_DIRT, startX, startY, right, 0.25, 3)
    {
        setVisible(true);
    }
};

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
