 #ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_


#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>
using namespace std;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{

	}
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	int getCurKey();
	void fillDirt();
	bool removeDirt(int x, int y);
	void HUD();
	int numOfBoulders();
	int numOfGoldNuggets();
	int numOfOilBarrels();
	int numOfSonarAndWaterTicks(); //Sonar and Water have the same formula
	int numOfProtesterTicksTillSpawn();
	bool isThereDirtVisibleHere(int x, int y);
	bool DMinVicinity(int range, int x, int y);
	bool ProtesterinVicinity(int range, int x, int y, char type);
	bool HCProtesterinVicinity(int range, int x, int y, char type);
	int dmXlocation() { return dm->getX(); }
	int dmYlocation() { return dm->getY(); }
	void generateField(string type);
	int randYGenerator(string type);
	int randXGenerator(string type);
	bool goodSpot(int randX, int randY);
	bool farAway(int x, int y);
	void dropNugget();
	void decrementGoldBait();
	void incrementGoldBait();
	int numOfGoldBait();
	void decrementSonarKit();
	void incrementSonarKit();
	int OilBarrelsRemaining();
	void decOilBarrels();
	int numOfSonarKits();
	int getSquirtsRemaining();
	void decrementSquirts();
	void incrementSquirts();
	void sonarBLAST();
	bool isDirtAboveMe(int x, int y, int z);
	bool isDirtLeftOfMe(int x, int y, int z);
	bool isDirtRightOfMe(int x, int y, int z);
	bool isDirtUnderMe(int x, int y, int z);
	bool getDistDigManOnX(int x, int y, int& dis);
	bool getDistDigManOnY(int x, int y, int& dis);
	bool canShout(int x, int y);
	bool isMoveableLocForProtester(int x, int y);
	bool isABoulderHere(int x, int y, GraphObject::Direction d);
	void addSquirtWeapon(GraphObject::Direction dir, int x, int y);
	void killProtestorsHere(int x, int y); 
	void killDm() { dm->decHealth(100); }
	bool isThereContact(int x, int y, int x2, int y2);
	bool dirtlessSpots(int x, int y);
	bool generateQuickPathField(int**& bfsArray, int x, int y);
private:
	int SonarKits;
	int GoldBait;
	int SquirtsRemaining;
	int OilBarrels;
	int Protesters;
	int ProtesterTicksPassed = 0;
	int currentKey;
	vector<Actor *> actors;
	DiggerMan* dm;
	Dirt* dirt[64][64];
	Protester* p;
	HardcoreProtester* hp;
};


#endif // STUDENTWORLD_H_
