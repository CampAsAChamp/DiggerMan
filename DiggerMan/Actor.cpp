#include "Actor.h"
#include "StudentWorld.h"
/*
----------------------------
DIGGERMAN IMPLEMENTATION 
----------------------------
*/
void DiggerMan::doSomething() {
	if (getWorld()->getLives() > 0) {
		moveDiggerMan();
		//cout << getX() << " " << getY() << endl;         <--- good test to see diggermans position when testing ranges
		if (clearDirt()) {
			getWorld()->playSound(SOUND_DIG);
		}
	}
}
void DiggerMan::moveDiggerMan() {
	int dir = getWorld()->getCurKey();
	switch (dir) {
	case KEY_PRESS_DOWN:
		if (getDirection() != down)
			setDirection(down);
		else if (getY() > 0 && !getWorld()->isABoulderHere(getX(), getY(), getDirection()))
			moveTo(getX(), getY() - 1);
		break;
	case KEY_PRESS_LEFT:
		if (getDirection() != left)
			setDirection(left);
		else if (getX() > 0 && !getWorld()->isABoulderHere(getX(), getY(), getDirection()))
			moveTo(getX() - 1, getY());
		break;
	case KEY_PRESS_RIGHT:
		if (getDirection() != right)
			setDirection(right);
		else if (getX() < VIEW_WIDTH - 4 && !getWorld()->isABoulderHere(getX(), getY(), getDirection()))
			moveTo(getX() + 1, getY());
		break;
	case KEY_PRESS_UP:
		if (getDirection() != up)
			setDirection(up);
		else if (getY() < VIEW_HEIGHT - 4 && !getWorld()->isABoulderHere(getX(), getY(), getDirection()))
			moveTo(getX(), getY() + 1);
		break;
	case KEY_PRESS_TAB:
		if (getWorld()->numOfGoldBait() > 0)
			getWorld()->dropNugget();
		break;
	case 'z':
		if (getWorld()->numOfSonarKits() > 0)
			getWorld()->sonarBLAST();
		break;
	case KEY_PRESS_SPACE:
		if (getWorld()->getSquirtsRemaining() > 0)
			getWorld()->addSquirtWeapon(getDirection(), getX(), getY());
	}
}
bool DiggerMan::clearDirt() {return getWorld()->removeDirt(getX(), getY()); }
/*
----------------------------
PROTESTER IMPLEMENTATION 
----------------------------
*/
Protester::Protester(StudentWorld * sw) :
	Character(IMID_PROTESTER, 60, 60, sw, left, 1.0, 0, 10), currentState(start), moveCount(0), waitCount(0), yellCoolDown(0), quickPathFound(false) {
	bfsArray = new int*[VIEW_WIDTH];
	for (int i = 0; i < VIEW_WIDTH; i++)
		bfsArray[i] = new int[VIEW_HEIGHT];
}
void Protester::doSomething() {
	if (isAlive()) {
		if (getHealth() <= 0 && waitCount <= 0)
			currentState = annoyed;
		//rest, move, annoyed, follow, start
		switch (currentState) {
		case start:		//init state
			startProt();
			break;
		case stunned:
			stunnedProt();
			break;
		case move:		//randomly moving state
			moveProt();
			break;
		case bait:
			considerBait();
		case annoyed:   //annoyed state, goBackToSafeSpace()
			leave();
			break;
		case rest:		//resting state, either between ticks, or when annoyed
			resting();
			break;
		}
		if (yellCoolDown != 0)
			yellCoolDown--;
	}
}
void Protester::chooseQuickDirection() {
	int u = 100000, d = 100000, l = 100000, r = 100000;
	if (getY() + 1 < 64 && bfsArray[getX()][getY() + 1] != -2)
		u = bfsArray[getX()][getY() + 1];
	if (getY() - 1 >= 0 && bfsArray[getX()][getY() - 1] != -2)
		d = bfsArray[getX()][getY() - 1];
	if (getX() + 1 < 64 && bfsArray[getX() + 1][getY()] != -2)
		r = bfsArray[getX() + 1][getY()];
	if (getX() - 1 >= 0 && bfsArray[getX() - 1][getY()] != -2)
		l = bfsArray[getX() - 1][getY()];
	if (u <= r && u <= l && u <= d) {
		setDirection(up);
		protesterMoveHelper(0, 1);
	}
	else if (r <= u && r <= l && r <= d) {
		setDirection(right);
		protesterMoveHelper(1, 0);
	}
	else if (l <= u && l <= r && l <= d) {
		setDirection(left);
		protesterMoveHelper(-1, 0);
	}
	else if (d <= u && d <= r && d <= l) {
		setDirection(down);
		protesterMoveHelper(0, -1);
	}
	waitCount = getTicksBetweenMoveCount();
	currentState = rest;
}
bool Protester::checkIfCanSeeDigMan() {
	int x;
	if (getWorld()->getDistDigManOnX(getX(), getY(), x)) {
		if (x < 0) {
			for (int i = 0; i > x; i--) {
				if (getWorld()->isDirtLeftOfMe(getX(), getY(), i)) 
					return false;
			}
			followDMHelper(getX() - 1, getY(), left);
			return true;
		}
		if (x > 0) {
			for (int i = 0; i < x; i++) {
				if (getWorld()->isDirtRightOfMe(getX(), getY(), i)) 
					return false;
			}
			followDMHelper(getX() + 1, getY(), right);
			return true;
		}
	}
	int y;
	if (getWorld()->getDistDigManOnY(getX(), getY(), y)) {
		if (y < 0) {
			for (int i = 0; i > y; i--) {
				if (getWorld()->isDirtUnderMe(getX(), getY(), i))
					return false;
			}
			followDMHelper(getX(), getY() - 1, down);
			return true;
		}
		if (y > 0) {
			for (int i = 0; i < y; i++) {
				if (getWorld()->isDirtAboveMe(getX(), getY(), i))
					return false;
			}
			followDMHelper(getX(), getY() + 1, up);
			return true;
		}
	}
	return false;
}
void Protester::startProt() {
	moveCount = getRandomDirMoveTickCount();
	waitCount = getTicksBetweenMoveCount();
	moveTo(getX() - 1, getY());
	currentState = rest;
}
void Protester::moveProt(){
	if (!checkIfCanSeeDigMan())
		moveProtester();
	if (yellCoolDown == 0 &&  getWorld()->canShout(getX(), getY())) {
		yellCoolDown = 15;
		stun();
	}
}
void Protester::stunnedProt() {
	if (getHealth() <= 0)
		currentState = annoyed;
	else if (time_stunned == 0)
		currentState = rest;
	else time_stunned--;
}

void Protester::considerBait() { 
	if (bait_time == 0)
		currentState = rest;
	else bait_time--;
}
void Protester::leave() {
	if (getX() == 60 && getY() == 60)
		kill();
	else
		goBackToSafeSpace();
}
void Protester::resting() {
	if (waitCount < 1)
		currentState = move;
	waitCount--;
}
void Protester::decHealth(int subAm) {
	if (health > 0) {
		health -= subAm;
		if (health <= 0) {
			quickPathFound = false;
			currentState = annoyed;
		}
	}
}
void Protester::followDMHelper(int x, int y, Direction d) {
	setDirection(d);
	moveTo(x, y);
	waitCount = getTicksBetweenMoveCount();
	moveCount = getRandomDirMoveTickCount();
	currentState = rest;
}
void Protester::goBackToSafeSpace(){
	if (quickPathFound) {
		chooseQuickDirection();
	}
	else {
		quickPathFound = getWorld()->generateQuickPathField(bfsArray, 60, 60);
	}
}
void Protester::protesterMoveHelper(int x, int y){
	moveTo(getX() + x, getY() + y);
	moveCount--;
}
void Protester::moveProtester(){
	if (waitCount < 1 && moveCount > 0) {
		if (getDirection() == left && getX() > 0 && !getWorld()->isDirtLeftOfMe(getX(), getY(), 0))
			protesterMoveHelper(-1, 0);
		else if (getDirection() == right && getX() < VIEW_WIDTH && !getWorld()->isDirtRightOfMe(getX(), getY(), 0))
			protesterMoveHelper(1, 0);
		else if (getDirection() == down && getY() > 0 && !getWorld()->isDirtUnderMe(getX(), getY(), 0))
			protesterMoveHelper(0, -1);
		else if (getDirection() == up && getY() < VIEW_HEIGHT - 4 && !getWorld()->isDirtAboveMe(getX(), getY(), 0))
			protesterMoveHelper(0, 1);
		else {
			setDirection(pickRandomDirection());
			moveCount = getRandomDirMoveTickCount();
		}
		currentState = rest;
		waitCount = getTicksBetweenMoveCount();
	}
	else {
		setDirection(pickRandomDirection());
		moveCount = getRandomDirMoveTickCount();
	}
}
void Protester::setStateAnnoyed() {
	currentState = annoyed;
}
Protester::~Protester() {
	for (int i = 0; i < VIEW_WIDTH; i++)
		delete[] bfsArray[i];
	delete[] bfsArray;
}
void Protester::stun() {
	time_stunned = max(50, int(100 - (10 * getWorld()->getLevel())));
	currentState = stunned; 
}
void Protester::baited() {
	bait_time = max(50, int(100 - (10 * getWorld()->getLevel())));
	currentState = bait;
}
Protester::Protester(StudentWorld * sw, int id, int x, int y, Direction dir, double size, int dep, int hp) : Character(id, x, y, sw, dir, size, dep, hp) {
	bfsArray = new int*[VIEW_WIDTH];
	for (int i = 0; i < VIEW_WIDTH; i++)
		bfsArray[i] = new int[VIEW_HEIGHT];
}
int Protester::getTicksBetweenMoveCount() { return max(0, 3 - (int)getWorld()->getLevel() / 4); }
int Protester::getRandomDirMoveTickCount() { return (rand() % 52) + 8; }
/*
----------------------------
NUGGET IMPLEMENTATION
----------------------------
*/
//=====Temp Nugget================================================================
void TempGoldNugget::doSomething() {
	if (!isAlive()) return;
	if (getTicksLeftTillDeath() == 0) {
		this->setVisible(false);
		this->kill();
	}
	else {
		if (getWorld()->ProtesterinVicinity(3, this->getX(), this->getY(), 'n')) {
			setVisible(false);
			this->kill();
			getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
			getWorld()->increaseScore(25);
		}
		else if (getWorld()->HCProtesterinVicinity(3, this->getX(), this->getY(), 'n')) {
			setVisible(false);
			this->kill();
			getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
			getWorld()->increaseScore(50);
		}
		decreaseLifeTicks();
	}
}
int TempGoldNugget::getTicksLeftTillDeath() { return ticksLeftTillDeath; }		//returns how many ticks i have till this object dies
void TempGoldNugget::decreaseLifeTicks() { ticksLeftTillDeath--; }				//decreases their ticks by one
//=====Perm Nugget================================================================
void PermGoldNugget::doSomething() {
	if (!isAlive())
		return;
	if (getWorld()->DMinVicinity(4, getX(), getY())) {
		this->setVisible(true);
	}
	if (getWorld()->DMinVicinity(3, getX(), getY())) {
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(10);
		getWorld()->incrementGoldBait();
		GoldPickedUp();
		this->setVisible(false);
		found = true;
		this->kill();
	}
}
void PermGoldNugget::GoldPickedUp() { found = true; }							//gold was found
/*
----------------------------
BOULDER IMPLEMENTATION
----------------------------
*/
void Boulder::doSomething() {
	switch (currentState) {
	case falling:
		if (getWorld()->isThereContact(getWorld()->dmXlocation(), getWorld()->dmYlocation(),getX(),getY())){
			getWorld()->killDm();
			currentState = done;
			this->setVisible(false);
			this->kill();
		}
		getWorld()->killProtestorsHere(getX(), getY()); //boulder can continue to fall after it hits a protester,idk why i split the functions up,can merge later

		if (getY() != 0 && !getWorld()->isDirtUnderMe(getX(), getY(), 0)){
			moveTo(getX(), getY() - 1);
		}
		else{
			currentState = done;
			this->setVisible(false);
			this->kill();
			getWorld()->playSound(SOUND_NONE);
		}
		break;
	case waiting:
		if (tickCount == 30) {
			getWorld()->playSound(SOUND_FALLING_ROCK);
			currentState = falling;
			getWorld()->removeDirt(getX(), getY());
		}
		else
			tickCount++;
		break;
	case stable:
		if (!getWorld()->isDirtUnderMe(getX(), getY(), 0))
			currentState = waiting;
		break;
	}
}
/*
----------------------------
BARREL IMPLEMENTATION
----------------------------
*/
void Barrel::doSomething(){
	if (!isAlive())
		return;
	if (getWorld()->DMinVicinity(4, getX(), getY())) {
		this->setVisible(true);
	}
	if (getWorld()->DMinVicinity(3, getX(), getY())) {
		getWorld()->playSound(SOUND_FOUND_OIL);
		getWorld()->increaseScore(1000);
		getWorld()->decOilBarrels();
		cout << getWorld()->OilBarrelsRemaining() << endl;
		this->setVisible(false);
		this->kill();
	}
}
/*
----------------------------
SONAR KIT IMPLEMENTATION
----------------------------
*/
void Sonar::doSomething() {
	if (!isAlive())
		return;
	if (getWorld()->DMinVicinity(3, getX(), getY())) {
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(75);
		getWorld()->incrementSonarKit();
		this->setVisible(false);
		this->kill();
	}
	else if (current_ticks() == 0) {
		this->setVisible(false);
		this->kill();
	}
	decrement_tick();
}
int Sonar::current_ticks() { return ticks; }
void Sonar::decrement_tick() { if (ticks > 0) ticks--; }
void Sonar::set_ticks() { ticks = getWorld()->numOfSonarAndWaterTicks(); }
/*
----------------------------
SQUIRT GUN IMPLEMENTATION
----------------------------
*/
void Squirt::doSomething() {
	if (!isAlive()) return;
	//protestorinvicinity checks if he is in range, and if he is, it damages him.
	if (getWorld()->ProtesterinVicinity(3, getX(), getY(), 's')){
		getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
		this->kill();
	}
	if (getWorld()->HCProtesterinVicinity(3, getX(), getY(), 's')) {
		getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
		this->kill();
	}
	if (squirt_distance == 8) {
		this->kill();
		return;
	}
	else {
		if (getDirection() == up) {
			if (!getWorld()->isDirtAboveMe(getWorld()->dmXlocation(), getWorld()->dmYlocation(), squirt_distance) && !getWorld()->isABoulderHere(getX(), getY(), getDirection())) {
				this->moveTo(getWorld()->dmXlocation(), getWorld()->dmYlocation()+1 + squirt_distance);
				incrementDistance();
			}
			else kill();
		}
		else if (getDirection() == down) {
			if (!getWorld()->isDirtUnderMe(getWorld()->dmXlocation(), getWorld()->dmYlocation(), squirt_distance) && !getWorld()->isABoulderHere(getX(), getY(), getDirection())) {
				this->moveTo(getWorld()->dmXlocation(), getWorld()->dmYlocation()-1 - squirt_distance);
				incrementDistance();
			}
			else kill();

		}
		else if (getDirection() == left) {
			if (!getWorld()->isDirtLeftOfMe(getWorld()->dmXlocation(), getWorld()->dmYlocation(), squirt_distance) && !getWorld()->isABoulderHere(getX(),getY(), getDirection())) {
				this->moveTo(getWorld()->dmXlocation()-1 - squirt_distance, getWorld()->dmYlocation());
				incrementDistance();
			}
			else kill();
		}
		else if (getDirection() == right) {
			if (!getWorld()->isDirtRightOfMe(getWorld()->dmXlocation(), getWorld()->dmYlocation(), squirt_distance) && !getWorld()->isABoulderHere(getX(), getY(), getDirection())) {
				this->moveTo(getWorld()->dmXlocation() + 1 + squirt_distance, getWorld()->dmYlocation());
				incrementDistance();
			}
			else kill();
		}
	}
}
/*
----------------------------
WATER POOL IMPLEMENTATION
----------------------------
*/
int WaterPool::getTicksLeftTillDeath() { return ticksLeftTillDeath; }		//returns how many ticks i have till this object dies
void WaterPool::decreaseLifeTicks() { ticksLeftTillDeath--; }				//decreases their ticks by one
void WaterPool::doSomething() {
	if (!isAlive()) return;
	if (getTicksLeftTillDeath() == 0) {
		this->setVisible(false);
		this->kill();
	}
	else {
		if (getWorld()->DMinVicinity(3, this->getX(), this->getY())) {
			setVisible(false);
			this->kill();
			getWorld()->playSound(SOUND_GOT_GOODIE);
			getWorld()->incrementSquirts();
			getWorld()->increaseScore(100);
		}
		decreaseLifeTicks();
	}
}
/*
----------------------------
HARDCORE PROTESTER IMPLEMENTATION
----------------------------
*/
HardcoreProtester::HardcoreProtester(StudentWorld * sw) : Protester(sw, IMID_HARD_CORE_PROTESTER, 60, 60, left, 1.0, 0, 20), followCount(0){
	currentState = start;
	moveCount = 0;
	waitCount = 0; 
	yellCoolDown = 0;
	quickPathFound = false;
}
void HardcoreProtester::doSomething(){
	if (isAlive()) {
		if (getHealth() <= 0 && waitCount <= 0)
			currentState = annoyed;			
		switch (currentState) {
		case start:		//init state
			startProt();
			break;
		case stunned:
			stunnedProt();
			break;
		case move:
			moveProt();
			break;
		case bait:
			considerBait();
			break;
		case annoyed:   //annoyed state, goBackToSafeSpace()
			leave();
			break;
		case follow:    //following diggerman
			break;
		case rest:		//resting state, either between ticks, or when annoyed
			resting();
			break;
		}
		if (yellCoolDown != 0)
			yellCoolDown--;
	}
}
