#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>

using namespace std;

//This is Nick's branch
const int MAXSIZE_X = 60;
const int MAXSIZE_Y = 64;

const int MINESHAFT_START_LEFT = 29;
const int MINESHAFT_STOP_RIGHT = 34;
const int MINESHAFT_BOTTOM = 4;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir)
    : GameWorld(assetDir)
    {}

	void setGameText()
	{
		string text;
		text = "Edit this stuff later";

		GameWorld::setGameStatText(text);
	}
    
    virtual int init()
    {
		srand(time(NULL));
		int randX = rand() % MAXSIZE_X;
		int randY = rand() % (MAXSIZE_Y - 8);
		cout << "boulder location X: " << randX << " Y: " << randY << endl;
		int boulderAtLVL = 3;

		for (int i = 0; i < MAXSIZE_Y; i++) //HORIZONTAL AXIS (X-Axis)
        {
			for (int j = 0; j < (MAXSIZE_X); j++) //VERTICAL AXIS (Y-Axis)
            {
                if (j < MINESHAFT_BOTTOM) //If below the mineshaft
                {
                    m_dirt[i][j] = new Dirt(this, i, j); //Add dirt to below the mineshaft
                }
                else
                {
                    if (i <= MINESHAFT_START_LEFT || i >= MINESHAFT_STOP_RIGHT) //If left or right to the mineshaft
                    {
						m_dirt[i][j] = new Dirt(this, i, j);
                    }
					else
					{ 
						m_dirt[i][j] = new Dirt(this, i, j);
						m_dirt[i][j]->setVisible(false);
					}
				
                }
            }
        }
		for (int i = 0; i < boulderAtLVL; i++)
		{
			if (randY < (MAXSIZE_Y - 8))
			{
				cout << "boulder location X: " << randX << " Y: " << randY << endl;
				m_actor[i][i] = new Boulder(this, randX, randY); //Testing to see if I can spawn a boulder
				for (int x = 0; x < 4; x++){
					for (int y = 0; y < 4; y++){
						m_dirt[randX + x][randY + y]->setVisible(false);
						cout << " errX: " << randX + x << " errY: " << randY + y << endl;
					}
				}
				randX = rand() % MAXSIZE_X;
				randY = rand() % (MAXSIZE_Y - 8);
			}
			else
			{
				randX = rand() % MAXSIZE_X;
				randY = rand() % (MAXSIZE_Y - 8);
				i--;
			}
		}
	
		m_diggerman = new DiggerMan(this);
        
        return GWSTATUS_CONTINUE_GAME;
    }
    
    void deleteDirt(int xPassed , int yPassed) //DOESNT ACTUALLY DELETE JUST SETS VISIBLE //WILL CLEAR LATER IN THE CLEAR ALL FUNCTION - Joseph
    {
        for (int x = xPassed; x < xPassed + 4; x++)
        {
            for (int y = yPassed; y < yPassed + 4; y++)
            {
				if (m_dirt[x][y] != nullptr)
				{
					if ((x < MAXSIZE_Y) && (y < MAXSIZE_X))
					{
						m_dirt[x][y]->setVisible(false);
					}
				}
				else
				{
					return;
				}
            }
        }
    }
    
    virtual int move()
    {
        // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
        // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
        
		StudentWorld::setGameText();
    
        m_diggerman->doSomething();

        return GWSTATUS_CONTINUE_GAME;
    }
    
    virtual void cleanUp()
    {}
    
private:
	Actor* m_actor[MAXSIZE_X][MAXSIZE_Y];
	Dirt * m_dirt[MAXSIZE_X][MAXSIZE_Y];
	DiggerMan* m_diggerman;
};

#endif // STUDENTWORLD_H_
