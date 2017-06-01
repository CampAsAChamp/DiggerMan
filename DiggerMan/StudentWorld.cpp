#include "StudentWorld.h"
#include <string>
#include <iomanip>
#include <sstream>

using namespace std;

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

void StudentWorld::setGameText()
{
    //Lvl: 52 Lives: 3 Hlth: 80% Wtr: 20 Gld: 3 Sonar: 1 Oil Left: 2 Scr: 321000

	ostringstream oss;
    oss << "Levl: " << m_level <<  " Lives: " << m_lives << " Health: " << "Water: " << m_diggerman->getWater() << " Gold: " << "Sonar: " << "Oil Left: " << "Score: " << getScore() << endl;

    string text = oss.str();
    
	GameWorld::setGameStatText(text);
}

int StudentWorld::init()
{
    if (m_lives == 3)
    {
        m_diggerman = new DiggerMan(this);
        srand(time(NULL));
        
        int randX = rand() % MAXSIZE_X;
        int randY = rand() % MAX_BOULDER_Y + Y_OFFSET;
        
        int barrelX = rand()%MAXSIZE_X;
        int barrelY = rand()%MAXSIZE_Y + Y_OFFSET;
        
        int pX= rand()%MAXSIZE_X;
        int pY = rand()%MAXSIZE_Y + Y_OFFSET;
        
//        m_actor[pX][pY] = new Protester(this, pX, pY);
        
        protester.push_back(new Protester(this, pX, pY));
        
        
        int barrelsAtLVL = min(2 + m_level, 18); //BARREL NUMBER SPAWN
        int boulderAtLVL = min(m_level / 2 + 2, 7);

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

        //BARREL HANDLER
        //*****BUGGY****** SOMETIMES FREEZES UP ON STARTUP
        for (int i = 0; i < barrelsAtLVL; i++)
        {
            cout << "BARREL COORDS: " << barrelX << " " << barrelY << endl;
            if (barrelX < (MAXSIZE_X - X_BOUND_RIGHT) && barrelY < (MAXSIZE_Y - Y_BOUND_TOP) && ItemDoesNotExist(barrelX, barrelY))
            {
            m_actor[barrelX][barrelY] = new Barrel(this, barrelX, barrelY);
            barrelX = rand()%MAXSIZE_X;
            barrelY = rand()%MAXSIZE_Y + Y_BOUND_TOP;
        
            }
            else
            {
                barrelX = rand() % MAXSIZE_X;
                barrelY = rand() % MAXSIZE_Y + Y_BOUND_TOP;
                i--;
            }
            
            
            
        }
        
        //BOULDER HANDLER
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
    }

	return GWSTATUS_CONTINUE_GAME;
}

bool StudentWorld::ItemDoesNotExist(int itemX, int itemY)
{
    
	bool objectDoesNotExist = false;

	for (int x = itemX; x < (itemX + 4); x++)
	{
		for (int y = itemY; y < (itemY + 4); y++)
		{
			if (m_dirt[x][y]->isVisible())
			{
				objectDoesNotExist = true;
			}
			else
			{
				objectDoesNotExist = false;
                return objectDoesNotExist;
			}
		}
	}
	return objectDoesNotExist;
}

void StudentWorld::deleteDirt(int xPassed, int yPassed) //DOESNT ACTUALLY DELETE JUST SETS VISIBLE //WILL CLEAR LATER IN THE CLEAR ALL FUNCTION - Joseph
{
    bool dirtDeleted = false;
	for (int x = xPassed; x < xPassed + 4; x++)
	{
		for (int y = yPassed; y < yPassed + 4; y++)
		{
			if (m_dirt[y][x] != nullptr)
			{
				if ((x < MAXSIZE_Y) && (y < MAXSIZE_X))
				{
					m_dirt[x][y]->setVisible(false);
                    dirtDeleted = true;
				}
			}
			else
			{
				return;
			}
		}
	}
    
    if (dirtDeleted == true)
    {
        playSound(SOUND_DIG);
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

bool StudentWorld::checkDirt(int xPassed, int yPassed)
{
    bool dirtFound = true;
    
    if (xPassed > 0 && xPassed < MAXSIZE_X && yPassed > 0 && yPassed < MAXSIZE_Y)
    {
        if (m_actor[xPassed][yPassed] != 0 || m_dirt[xPassed][yPassed]->isVisible())
        {
            dirtFound = true;
        }
        else
        {
            dirtFound = false;
        }
    }
    else
    {
        dirtFound = true;
    }
    
    return dirtFound;
}

bool StudentWorld::checkBoulderBelow(int xPassed, int yPassed)
{
	bool boulderFound = false;

	for (int xToCheck = xPassed; xToCheck < xPassed + 4; xToCheck++)
	{
		if (m_actor[xToCheck][yPassed - 4] != 0)
		{
			if (m_actor[xToCheck][yPassed - 4]->isVisible())
			{
				boulderFound = true;
				return true;
			}
		}
		else
		{
			continue;
		}
	}
	return false;
}

bool StudentWorld::checkDiggermanBelow(int xPassed, int yPassed)
{
	if ((yPassed - 4) == m_diggerman->getY() && xPassed == m_diggerman->getX())
		return true;
	else
		return false;
	
}

void StudentWorld::setDiggermanHP(int hitPoints)
{
	m_diggerman->setHitpoints(hitPoints);
	cout << "\tDiggerman is dead\n";
}

void StudentWorld::removeDeadActors()
{
	for (int i = 0; i < MAXSIZE_X; i++)
	{
		for (int j = 0; j < MAXSIZE_Y; j++)
		{
			if (m_actor[i][j] != 0 && !m_actor[i][j]->isAlive())
			{
				m_actor[i][j]->setVisible(false);
				m_actor[i][j] = 0;
				cout << "\tDeleted actor at " << i << " | " << j << endl;
				delete m_actor[i][j];
			}
			else
			{
				continue;
			}
		}
	}
    
    for (size_t h = 0; h < protester.size(); h++)
    {
        if (protester[h] != nullptr && !protester[h]->isAlive())
        {
            protester[h]->setState(leaveOilField);
            protester[h]->setVisible(false);
            cout << "\tDeleted protester at at " << protester[h]->getX() << " | " << protester[h]->getY() << endl;
        }
        else
        {
            continue;
        }
    }
    
	if (!m_diggerman->isAlive())
	{
		m_diggerman->setVisible(false);
		//m_diggerman = 0;
		cout << "\tDeleted DiggerMan at " << m_diggerman->getX() << " | " << m_diggerman->getY() << endl;
		//delete m_diggerman;
	}
}

int StudentWorld::move()
{
    if (m_lives == 0)
    {
        exit(0);
    }
    
    ticks++;
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.

	StudentWorld::setGameText();
	m_diggerman->doSomething(); //Diggerman doSomething
    
    //WILL REPLACE '1' LATER WITH CURRENT_LEVEL_NUM
    int G = m_level * 25 + 300; //1 in G CHANCE THAT A WATERPOOL WILL SPAWN BASED ON TICKS
    
    if (ticks%G == 0) //IF TICKS PASSED IS G CHANCE THEN SPAWN WATERPOOL
    {
        int x = rand()%MAXSIZE_X;
        int y = rand()%MAXSIZE_Y - Y_BOUND_TOP;
        
        m_actor[x][y] = new WaterPool(this, x, y);
    }
    
    bool itemFound = checkDirt(m_diggerman->getX(), m_diggerman->getY());
    if (itemFound)
    {
        cout << "THERE IS AN ITEM FOUND ON DIGGERMAN's POSITION" << endl;
    }

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
    
    if (!m_diggerman->isAlive())
    {
        m_lives--;
        m_diggerman->setHitpoints(1);
        m_diggerman->setVisible(true);
        
        return GWSTATUS_PLAYER_DIED;
    }

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::squirt(int xPassed, int yPassed, DiggerMan::Direction dir)
{
    /* if (checkDirt(xPassed, yPassed + 1) == true || checkDirt(xPassed, yPassed - 1) == true || checkDirt(xPassed + 1, yPassed) == true || checkDirt(xPassed - 1, yPassed) == true)
    {
        return;
    } */ 
    
    cout << "Water: " << m_diggerman->getWater() << endl;
    
    if (checkProtester(xPassed, yPassed, DiggerMan::up))
    {
        cout << "HIT" << endl;
    }
    
    if (xPassed > -1 && xPassed < MAXSIZE_X && yPassed > -1 && yPassed < MAXSIZE_Y)
    {
        if (m_diggerman->getWater() > 0)
        {
            switch (dir)
            {
                case DiggerMan::up :
                    m_actor[xPassed][yPassed] = new Squirt(this, xPassed, yPassed + 4, DiggerMan::up);
                    m_actor[xPassed][yPassed]->setDirection(DiggerMan::up);
                    playSound(SOUND_PLAYER_SQUIRT);
                    m_diggerman->reduceWater();
        
                    return;
    
                case DiggerMan::down :
                    m_actor[xPassed][yPassed] = new Squirt(this, xPassed, yPassed - 4, DiggerMan::down);
                    m_actor[xPassed][yPassed]->setDirection(DiggerMan::down);
                    playSound(SOUND_PLAYER_SQUIRT);
                    m_diggerman->reduceWater();
                    return;
        
                case DiggerMan::right :
                    m_actor[xPassed][yPassed] = new Squirt(this, xPassed + 4, yPassed, DiggerMan::right);
                    m_actor[xPassed][yPassed]->setDirection(DiggerMan::right);
                    playSound(SOUND_PLAYER_SQUIRT);
                    m_diggerman->reduceWater();
                    return;
        
                case DiggerMan::left :
                    m_actor[xPassed][yPassed] = new Squirt(this, xPassed - 4, yPassed, DiggerMan::left);
                    m_actor[xPassed][yPassed]->setDirection(DiggerMan::left);
                    playSound(SOUND_PLAYER_SQUIRT);
                    m_diggerman->reduceWater();
                    return;
        
                default:
                    m_actor[xPassed][yPassed]->setHitpoints(0);
                    return;
            }
        }
        else
        {
            return;
        }
    }
    else
    {
        return;
    }
}

bool StudentWorld::checkDiggerman(int xPassed, int yPassed, DiggerMan::Direction dir)
{
    bool diggermanFound = false;
    
    //Check Above
    for (int i = 1; i < 4; i++)
    {
        if (xPassed + i == m_diggerman->getX() && yPassed == m_diggerman->getY())
            return true;
        else if (xPassed - i == m_diggerman->getX() && yPassed == m_diggerman->getY())
            return true;
        else if (xPassed == m_diggerman->getX() && yPassed + i == m_diggerman->getY())
            return true;
        else if (xPassed == m_diggerman->getX() && yPassed - i == m_diggerman->getY())
            return true;
    }
    
    /*
     if (diggermanFound)
     {
     switch (dir)
     {
     case Protester::left:
     if (m_diggerman->getX() < xPassed)
     return true;
     
     case Protester::right:
     if (m_diggerman->getX() > xPassed)
     return true;
     
     case Protester::up:
     if (m_diggerman->getY() > yPassed)
     return true;
     
     case Protester::down:
     if (m_diggerman->getY() < yPassed)
     return true;
     
     default:
     return false;
     }
     }
    */
    
    
    return diggermanFound;
}

//*****BUGGY***** NOT SURE IF WORKS
bool StudentWorld::checkProtester(int xPassed, int yPassed, Protester::Direction dir)
{
    bool protesterFound = false;
    
    //Check Above

    for (size_t i = 0; i < protester.size(); i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (protester[i] != 0)
            {
                if (xPassed + j == protester[i]->getX() && yPassed == protester[i]->getY())
                {
                    setIndex(i);
                    protesterFound = true;
                }
                if (xPassed - j == protester[i]->getX() && yPassed == protester[i]->getY())
                {
                    setIndex(i);
                    protesterFound = true;
                }
                if (xPassed == protester[i]->getX() && yPassed + j == protester[i]->getY())
                {
                    setIndex(i);
                    protesterFound = true;
                }
                if (xPassed == protester[i]->getX() && yPassed - j == protester[i]->getY())
                {
                    setIndex(i);
                    protesterFound = true;
                }
                
                if (xPassed == protester[i]->getX() && yPassed == protester[i]->getY())
                {
                    setIndex(i);
                    protesterFound = true;
                }
            }
            else
            {
                continue;
            }
        }
    }
    
    return protesterFound;
}

void StudentWorld::cleanUp()
{}
