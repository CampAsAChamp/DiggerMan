#include "StudentWorld.h"
#include <string>
using namespace std;


// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

void StudentWorld::setGameText()
{
	string text;
	text = "Edit this stuff later";

	GameWorld::setGameStatText(text);
}

int StudentWorld::init()
{
	srand(time(NULL));
	int randX = rand() % MAXSIZE_X;
	int randY = rand() % MAX_BOULDER_Y + Y_OFFSET;
	int boulderAtLVL = 3;

	for (int i = 0; i < MAXSIZE_X; i++) //VERTICAL AXIS (X-Axis)
	{
		for (int j = 0; j < (MAXSIZE_Y); j++) //HORIZONTAL AXIS (Y-Axis)
		{
			m_actor[i][j] = 0; //Initializing all actors to 0 in order to check if there is an actual actor there
		}
	}

	for (int x = 0; x < MAXSIZE_X; x++) //VERTICAL AXIS (X-Axis)
	{
		for (int y = 0; y < (MAXSIZE_Y); y++) //HORIZONTAL AXIS (Y-Axis)
		{
			m_dirt[x][y] = new Dirt(this, x, y);

			if (x > MINESHAFT_START_LEFT && x < MINESHAFT_STOP_RIGHT) //If below the mineshaft
			{
				if (y > MINESHAFT_BOTTOM)
				{
					m_dirt[x][y]->setVisible(false);
				}
			}

			if (y > MAXSIZE_Y - 5)
			{
				m_dirt[x][y]->setVisible(false);
			}
		}
	}

	for (int i = 0; i < boulderAtLVL; i++)
	{
		if (randX < (MAXSIZE_X - X_BOUND_RIGHT) && randY < (MAXSIZE_Y - Y_BOUND_TOP) && ItemDoesNotExist(randX,randY))
		{
			m_actor[randX][randY] = new Boulder(this, randX, randY);
			deleteDirt(randX, randY);
			randX = rand() % MAXSIZE_X;
			randY = rand() % MAX_BOULDER_Y + Y_OFFSET;
		}
		else
		{
			randX = rand() % MAXSIZE_X;
			randY = rand() % MAX_BOULDER_Y + Y_OFFSET;
			i--;
		}
	}

	m_diggerman = new DiggerMan(this);

	return GWSTATUS_CONTINUE_GAME;
}
bool StudentWorld::ItemDoesNotExist(int itemX, int itemY)
{
	bool objectExist = false;

	for (int x = itemX; x < (itemX + 4); x++)
	{
		for (int y = itemY; y < (itemY + 4); y++)
		{
			if (m_dirt[x][y]->isVisible())
			{
				objectExist = true;
				break;
			}
			else
			{
				objectExist = false;
				return objectExist;
			}
		}
	}
	return objectExist;
}

void StudentWorld::deleteDirt(int xPassed, int yPassed) //DOESNT ACTUALLY DELETE JUST SETS VISIBLE //WILL CLEAR LATER IN THE CLEAR ALL FUNCTION - Joseph
{
	for (int x = xPassed; x < xPassed + 4; x++)
	{
		for (int y = yPassed; y < yPassed + 4; y++)
		{
			if (m_dirt[y][x] != nullptr)
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

bool StudentWorld::checkDirtBelow(int xPassed, int yPassed)
{
	bool dirtFound = true;

	for (int xToCheck = xPassed; xToCheck < xPassed + 4; xToCheck++)
	{
		if (m_dirt[xToCheck][yPassed - 1]->isVisible())
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

bool StudentWorld::checkActorBelow(int xPassed, int yPassed, int IMID)
{
	bool found = true;

	for (int xToCheck = xPassed; xToCheck < xPassed + 4; xToCheck++)
	{
		if (IMID == IMID_DIRT)
		{
			return checkDirtBelow(xPassed, yPassed); //Has to call different function as the array to check is different
		}
		else if (m_actor[xToCheck][yPassed - 1] != 0)
		{
			if (m_actor[xToCheck][yPassed - 1]->getID() == IMID && m_actor[xToCheck][yPassed - 1]->isVisible())
			{
				found = true;
				break;
			}
			else
			{
				found = false;
			}
		}
		else
			continue;
	}
	return found;
}
int StudentWorld::move()
{
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.

	StudentWorld::setGameText();
	m_diggerman->doSomething(); //Diggerman doSomething

	for (int i = 0; i < MAXSIZE_X; i++)
	{
		for (int j = 0; j < MAXSIZE_Y; j++)
		{
			if (m_actor[i][j] != 0)
			{
				m_actor[i][j]->doSomething(); //Call doSomething for all actors
			}
			else
			{
				continue;
			}
		}
	}
	removeDeadActors(); //Checks every tick to remove the actors that are dead
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::removeDeadActors()
{
	for (int i = 0; i < MAXSIZE_X; i++)
	{
		for (int j = 0; j < MAXSIZE_Y; j++)
		{
			if (m_actor[i][j] != 0 && !m_actor[i][j]->isAlive())
			{
				m_actor[i][j]->setVisible(false); //Remove all dead actors
			}
			else
			{
				continue;
			}
		}
	}
}

void StudentWorld::cleanUp()
{}
