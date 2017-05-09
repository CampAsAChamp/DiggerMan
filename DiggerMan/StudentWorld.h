#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <iostream>

using namespace std;

//This is Nick's branch
const int MAXSIZE_X = 60;
const int MAXSIZE_Y = 64;

const int MINESHAFT_START_LEFT = 29;
const int MINESHAFT_STOP_RIGHT = 34;
const int MINESHAFT_BOTTOM = 16;

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
		for (int i = 0; i < MAXSIZE_X; i++) //VERTICAL AXIS (X-Axis)
		{
			for (int j = 0; j < (MAXSIZE_Y); j++) //HORIZONTAL AXIS (Y-Axis)
			{
				if (i < MINESHAFT_BOTTOM) //If below the mineshaft
				{
					m_dirt[i][j] = new Dirt(this, j, i); //Add dirt to below the mineshaft
				}
				else
				{
					if (j <= MINESHAFT_START_LEFT || j >= MINESHAFT_STOP_RIGHT) //If left or right to the mineshaft
					{
						m_dirt[i][j] = new Dirt(this, j, i);
					}
					else
					{
						m_dirt[i][j] = new Dirt(this, j, i);
						m_dirt[i][j]->setVisible(false);
					}

				}
			}
		}

		for (int i = 0; i < MAXSIZE_X; i++) //VERTICAL AXIS (X-Axis)
		{
			for (int j = 0; j < (MAXSIZE_Y); j++) //HORIZONTAL AXIS (Y-Axis)
			{
				m_actor[i][j] = 0; //Initializing all actors to 0 in order to check if there is an actual actor there
			}
		}

		m_actor[40][20] = new Boulder(this, 40, 20); //Testing to see if I can spawn a boulder

		m_diggerman = new DiggerMan(this);

		return GWSTATUS_CONTINUE_GAME;
	}

	void deleteDirt(int xPassed, int yPassed) //DOESNT ACTUALLY DELETE JUST SETS VISIBLE //WILL CLEAR LATER IN THE CLEAR ALL FUNCTION - Joseph
	{
		for (int x = xPassed; x < xPassed + 4; x++)
		{
			for (int y = yPassed; y < yPassed + 4; y++)
			{
				if (m_dirt[y][x] != nullptr)
				{
					if ((x < MAXSIZE_Y) && (y < MAXSIZE_X))
					{
						m_dirt[y][x]->setVisible(false);
					}
				}
				else
				{
					return;
				}
			}
		}
	}

	bool checkDirtBelow(int xPassed, int yPassed)
	{
		bool dirtFound = true;

		for (int xToCheck = xPassed; xToCheck < xPassed + 4; xToCheck++)
		{
			if (m_dirt[yPassed - 1][xToCheck]->isVisible())
			{
				dirtFound = true;
				break;
			}
			else
			{
				dirtFound = false;
			}
		}
		return dirtFound;
	}

	virtual int move()
	{
		// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.

		StudentWorld::setGameText();

		m_diggerman->doSomething(); //Diggerman doSomething

		for (int i = 0; i < MAXSIZE_X; i++) //VERTICAL AXIS (X-Axis)
		{
			for (int j = 0; j < MAXSIZE_Y; j++) //HORIZONTAL AXIS (Y-Axis)
			{
				if (m_actor[i][j] != 0)
				{
					m_actor[i][j]->doSomething(); //Call doSomething for actor array
				}
				else
				{
					continue;
				}
			}
		}
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual void cleanUp()
	{}

private:
	Actor * m_actor[MAXSIZE_X][MAXSIZE_Y];
	Dirt * m_dirt[MAXSIZE_X][MAXSIZE_Y];
	DiggerMan* m_diggerman;

};

#endif // STUDENTWORLD_H_
