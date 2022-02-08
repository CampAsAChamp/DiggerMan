#ifndef ACTOR_H_
#define ACTOR_H_


#include "GraphObject.h"
#include <algorithm>
#include <queue>
#include <stack>
using namespace std;
class StudentWorld;

class Actor : public GraphObject {
public:
	Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* sw) :
		GraphObject(imageID, startX, startY, dir, size, depth), world(sw), alive(true) {};
	virtual void doSomething() {};
	StudentWorld* getWorld() const { return world; }
	bool isAlive() const { return alive; }
	virtual bool isRegProtester() { return false; }
	virtual bool isHCProtester() { return false; }
	void kill() { alive = false; }	
protected:
private:
	StudentWorld* world;
	bool alive;
};


class Character : public Actor {
public:
	Character(int id, int x, int y, StudentWorld* sw, Direction dir, double size, int dep, int hp) : 
		Actor(id, x, y, dir, size, dep, sw), health(hp) {}
	int getHealth() const { return health; }
	virtual void decHealth(int subAm) { health -= subAm; }
protected:
	int health;
};


class DiggerMan : public Character {
public:
	DiggerMan(StudentWorld* sw) : 
		Character(IMID_PLAYER, 30, 60, sw, right, 1.0, 0, 10) {};
	virtual void doSomething();
private:
	void moveDiggerMan();
	bool clearDirt();
};


class Protester : public Character {
protected:
	enum State { rest, move, annoyed, stunned, follow, start, bait };
	int** bfsArray;
	void chooseQuickDirection();
	bool quickPathFound;	
	int yellCoolDown;
	int time_stunned;	
	int bait_time;
	int moveCount;
	int waitCount;
	State currentState;
	Direction pickRandomDirection() {
		int i = rand() % 4;
		if (i == 0)
			return left;
		if (i == 1)
			return right;
		if (i == 2)
			return down;
		if (i == 3)
			return up;
		return none;
	}
	void followDMHelper(int x, int y, Direction d);	 
	void protesterMoveHelper(int x, int y);
	void moveProtester();
	bool checkIfCanSeeDigMan();
	void startProt();
	void moveProt();
	void considerBait();
	void stunnedProt();
	void leave();
	void resting();
public:	
	void decHealth(int subAm);
	int getTicksBetweenMoveCount();
	int getRandomDirMoveTickCount();
	void baited();
    void stun();   
	Protester(StudentWorld* sw, int id, int x, int y, Direction dir, double size, int dep, int hp);
	Protester(StudentWorld * sw);
	void doSomething();
	void goBackToSafeSpace();
	virtual bool isRegProtester() { return true;}
	void setStateAnnoyed();
	~Protester();
private:
};


class HardcoreProtester : public Protester {
public:
	HardcoreProtester(StudentWorld *sw);
	void doSomething();
	virtual bool isRegProtester() { return false; }
	virtual bool isHCProtester() { return true; }
private:
	int followCount;
};


class Dirt : public Actor {
public:
	Dirt(int startX, int startY, StudentWorld* sw) :
		Actor(IMID_DIRT, startX, startY, right, .25, 3, sw) {};
	virtual void doSomething() {};
};

class Goodies :public Actor {
public:
	Goodies(StudentWorld* sw, const int img, int randX, int randY) :
		Actor(img, randX, randY, right, 1.0, 2, sw) {};
	virtual void doSomething() {};
};


class Boulder : public Actor {
public:
	enum State { stable, falling, waiting, done };
	Boulder(int x, int y, StudentWorld* sw) : 
		Actor(IMID_BOULDER, x, y, down, 1.0, 1, sw), currentState(stable), tickCount(0){ };
	virtual void doSomething();
	State getState() const { return currentState; }
private:
	State currentState;
	int tickCount;
};


class Barrel : public Goodies{
public:
	Barrel(int randX, int randY, StudentWorld* sw) : 
		Goodies(sw, IMID_BARREL, randX, randY){};
	virtual void doSomething();
};


class Sonar : public Goodies {
public:
	Sonar(int randX, int randY, StudentWorld* sw) :
		Goodies(sw, IMID_SONAR, randX, randY) {
		set_ticks();
	};
	virtual void doSomething();
	int  current_ticks();
	void set_ticks();
	void decrement_tick();
private:
	int ticks;
};


class TempGoldNugget : public Goodies {
public:
	TempGoldNugget(int deathTicks, int randX, int randY, StudentWorld* sw) :
		Goodies(sw, IMID_GOLD, randX, randY), ticksLeftTillDeath(deathTicks) { this->setVisible(true); };
	int getTicksLeftTillDeath();
	void decreaseLifeTicks();
	virtual void doSomething();
private:
	int ticksLeftTillDeath;
};


class PermGoldNugget : public Goodies {
public:
	PermGoldNugget(StudentWorld* sw, int randX, int randY) :
		Goodies(sw, IMID_GOLD, randX, randY) {};
	void GoldPickedUp();
	virtual void doSomething();
private:
	bool found = false;
};


class WaterPool : public Goodies {
public:
	WaterPool(int deathTicks, int randX, int randY, StudentWorld* sw) :
		Goodies(sw, IMID_WATER_POOL, randX, randY), ticksLeftTillDeath(deathTicks) {};
	int getTicksLeftTillDeath();
	void decreaseLifeTicks();
	virtual void doSomething();
private:
	int ticksLeftTillDeath;
};


class Squirt : public Actor { //cannot inherit from goodies because depth is 1 not 2
public:
	Squirt(StudentWorld* sw, Direction d, int x, int y) :
		Actor(IMID_WATER_SPURT, x, y, d, 1, 1, sw) { squirt_distance = 0; };
	virtual void doSomething();
	void incrementDistance() { squirt_distance++; }
private:
	int squirt_distance;
};


#endif // ACTOR_H_
