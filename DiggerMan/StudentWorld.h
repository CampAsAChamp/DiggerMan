#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <vector>

using namespace std;

const int MAXSIZE_X = 64;
const int MAXSIZE_Y = 64;

const int MINESHAFT_START_LEFT = 29;
const int MINESHAFT_STOP_RIGHT = 34;
const int MINESHAFT_BOTTOM = 4;
const int OBJECT_EXIST = 1;
const int BOULDER_EXIST = 2;
const int Y_OFFSET = 20;
const int Y_BOUND_TOP = 8;
const int X_BOUND_RIGHT = 4;
const int MAX_BOULDER_Y = 38;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class DiggerMan; //FORWARD DECLARATION

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir)
        : GameWorld(assetDir)
    {
        ticks = 0;
        m_level = 0;
        m_lives = 3;
        protesterIndex = 0;
    }

    void setGameText();
    virtual int init();
    virtual int move();
    virtual void cleanUp();

	void removeDeadActors();

    void deleteDirt(int xPassed, int yPassed); 
    //bool checkActorBelow(int xPassed, int yPassed, int IMID);
	bool checkBoulderBelow(int xPassed, int yPassed);
    bool checkDiggerman(int xPassed, int yPassed, Actor::Direction dir);
	bool checkDiggermanBelow(int xPassed, int yPassed);
    bool checkDirt(int xPassed, int yPassed);
    bool checkDirtBelow(int xPassed, int yPassed);
    bool checkProtester(int xPassed, int yPassed, Protester::Direction dir);
    bool protesterCheckDiggerman(int xPassed, int yPassed);
    bool protesterFacingDiggerman(int xPassed, int yPassed, Protester::Direction dir);
	void setDiggermanHP(int hitPoints);
    inline void annoyDiggerman(int hitPoints)
    {
        int tempHP = m_diggerman->getHitpoints();
        tempHP = tempHP - hitPoints;
        m_diggerman->setHitpoints(tempHP);
    }
    
    inline void annoyProtester(int hitPoints)
    {
        int index = getIndex();

        int tempHP = protester[index]->getHitpoints();
        tempHP = tempHP - hitPoints;
        protester[index]->setHitpoints(tempHP);
        playSound(SOUND_PROTESTER_ANNOYED);
        
        cout << "P LIFE" << protester[index]->getHitpoints() << endl;
        
    }
    bool ItemDoesNotExist(int itemX, int itemY);
      void squirt(int xPassed, int yPassed, Actor::Direction dir);

    inline DiggerMan* getDiggerMan()
    {
        return m_diggerman;
    }
    inline int getLevel()
    {
        return m_level;
    }
    inline int getTicks()
    {
        return ticks;
    }
    inline void setIndex(int index)
    {
        this->protesterIndex = index;
    }
    inline int getIndex()
    {
        return protesterIndex;
    }

    
private:
    Actor * m_actor[MAXSIZE_X][MAXSIZE_Y];
    Dirt * m_dirt[MAXSIZE_X][MAXSIZE_Y];
    DiggerMan* m_diggerman;
    vector<Protester*> protester;
    
    int ticks;
    int protesterIndex;
    int m_level;
    int m_lives;
};

#endif // STUDENTWORLD_H_
