#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

/*

You must create a class called StudentWorld that is responsible for keeping track
of your game world (including the oil field) and all of its actors/objects (e.g., the
DiggerMan, Protesters, Boulders, Dirt, etc.) that are inside the oil field.

*/
