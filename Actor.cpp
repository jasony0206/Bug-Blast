#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

//Coord class is used in a ComplexZumi function where it finds the direction it should move to 
//in order to reach the player in the shortest path
class Coord
{
  public:
	Coord(int rr, int cc, int dir) : m_r(rr), m_c(cc), m_dir(dir){}
	int r() const { return m_r; }
	int c() const { return m_c; }
	int getDir()const	{return m_dir;}
  private:
	int m_r;
	int m_c;
	int m_dir;		//x, y + direction
};


/////////////////////////
//ACTOR FUNCTIONS (ABC)
/////////////////////////
Actor::Actor(StudentWorld *swptr, int imageID, int startX, int startY)
	:GraphObject(imageID, startX, startY){
	m_swp = swptr;
	m_alive = true;
	setVisible(true);	//Every actor(except Exit) should be visible when created
}

Actor::~Actor(){
}

StudentWorld* Actor::getWorld(){
	return m_swp;
}

void Actor::setDead(){
	m_alive = false;
}

bool Actor::alive(){
	return m_alive;
}


/////////////////////////
//PLAYER FUNCTIONS
/////////////////////////
Player::Player(StudentWorld* swptr, int startX, int startY)
	:Actor(swptr, IID_PLAYER, startX, startY){
		m_numSprayers = 0;
		m_maxSprayers = 2;
		m_WalkThruGoodieCount = 0;
		m_IncreaseSSGoodieCount = 0;
}

Player::~Player(){
}

void Player::incMaxSprays(){
	m_maxSprayers = getWorld()->getMaxBoostedSprayers();}

void Player::resetMaxSprays(){
	m_maxSprayers = 2;}

void Player::decCurNumSprayers(){
	m_numSprayers--;}

void Player::gotWalkThruGoodie(){
	m_WalkThruGoodieCount = getWorld()->getWalkThruGoodieLifetimeTicks();}

void Player::gotIncreaseSSGodie(){
	m_IncreaseSSGoodieCount = getWorld()->getBoostedSprayerLifetimeTicks();}


void Player::doSomething(){
	//check goodie effect counts
	if(m_IncreaseSSGoodieCount == getWorld()->getBoostedSprayerLifetimeTicks())
		incMaxSprays();
	if(m_IncreaseSSGoodieCount > 0)
		m_IncreaseSSGoodieCount--;
	else if(m_IncreaseSSGoodieCount == 0)
		resetMaxSprays();

	if(m_WalkThruGoodieCount > 0)
		m_WalkThruGoodieCount--;
	else if(m_WalkThruGoodieCount == 0 
		&& getWorld()->content(getX(), getY()) == "destroyable brick"){
		setDead();
		getWorld()->decLives();
	}

	//make a designated move if possible
	int key;
	if(getWorld()->getKey(key)){
		std::string what;
		switch(key)
		{
		case KEY_PRESS_DOWN:			
			what = getWorld()->content(getX(), getY()-1);
			if(what == "permanent brick"
				||(m_WalkThruGoodieCount == 0 && what == "destroyable brick"))	
				break;
			moveTo(getX(), getY()-1);
			break;
		case KEY_PRESS_LEFT:			
			what = getWorld()->content(getX()-1, getY());
			if(what == "permanent brick"
				||(m_WalkThruGoodieCount == 0 && what == "destroyable brick"))
				break;			
			moveTo(getX()-1, getY());
			break;
		case KEY_PRESS_RIGHT:
			what = getWorld()->content(getX()+1, getY());
			if(what == "permanent brick"
				||(m_WalkThruGoodieCount == 0 && what == "destroyable brick"))
				break;			
			moveTo(getX()+1, getY());
			break;
		case KEY_PRESS_UP:
			what = getWorld()->content(getX(), getY()+1);
			if(what == "permanent brick"
				||(m_WalkThruGoodieCount == 0 && what == "destroyable brick"))
				break;			
			moveTo(getX(), getY()+1);
			break;
		case KEY_PRESS_SPACE:
			//drop a sprayer
			//if there's already a sprayer or a brick, or already max sprayers, do nth
			if(getWorld()->content(getX(), getY()) == "bug sprayer" 
				|| getWorld()->content(getX(), getY()) == "destroyable brick" 
				|| m_numSprayers == m_maxSprayers)
				break;
			getWorld()->drop("bug sprayer", getX(), getY());
			m_numSprayers++;
			break;
		default:
			break;
		}
	}
}


/////////////////////////
//BRICK FUNCTIONS (ABC)
/////////////////////////
Brick::Brick(StudentWorld* swptr, int imageID, int startX, int startY)
	:Actor(swptr, imageID, startX, startY){
}

Brick::~Brick(){
}


///////////////////////////
//PERMANENT BRICK FUNCTIONS
///////////////////////////
PermaBrick::PermaBrick(StudentWorld* swptr, int startX, int startY)
	:Brick(swptr, IID_PERMA_BRICK, startX, startY){
}

PermaBrick::~PermaBrick(){
}

void PermaBrick::doSomething(){		//PermaBrick shouldn't do anything
}


///////////////////////////
//DESTROYABLE BRICK FUNCTIONS
///////////////////////////
DestBrick::DestBrick(StudentWorld* swptr, int startX, int startY)
	:Brick(swptr, IID_DESTROYABLE_BRICK, startX, startY){
}

DestBrick::~DestBrick(){
}

void DestBrick::doSomething(){
	//if bug spray in its square, dead
	if(getWorld()->content(getX(), getY()) == "bug spray"){
		setDead();
		getWorld()->erase4maze(getX(), getY());
	}
}


///////////////////////////
//EXIT FUNCTIONS
///////////////////////////
Exit::Exit(StudentWorld* swptr, int startX, int startY)
	:Actor(swptr, IID_EXIT, startX, startY)
{
	exitVisible = false;
	setVisible(false);	//not visible when constructed (only exception)
	//this only happens once (these conditions take care of it)
	if(!exitVisible && getWorld()->numZumi() == 0 && getWorld()->clear()){
		setVisible(true);
		getWorld()->playSound(SOUND_REVEAL_EXIT);
		exitVisible = true;
	}
	if(getWorld()->sameSquareAsPlayer(getX(), getY()) && exitVisible && !getWorld()->complete()){
		getWorld()->playSound(SOUND_FINISHED_LEVEL);
		getWorld()->setComplete();	//lvl completed
	}
}

Exit::~Exit(){
}

void Exit::doSomething(){
	
	if(!exitVisible && getWorld()->numZumi() == 0 && getWorld()->clear()){
		setVisible(true);
		getWorld()->playSound(SOUND_REVEAL_EXIT);
		exitVisible = true;
	}
	if(getWorld()->sameSquareAsPlayer(getX(), getY()) && exitVisible && !getWorld()->complete()){
		getWorld()->playSound(SOUND_FINISHED_LEVEL);
		getWorld()->setComplete();	//lvl completed
	}
}

////////////////////////
//TIMED OBJECT FUNCTIONS
////////////////////////

TimedObject::TimedObject(StudentWorld* swptr, int imageID, int startX, int startY, int lifetime)
	:Actor(swptr, imageID, startX, startY)
{
	m_lifetime = lifetime;
}

TimedObject::~TimedObject(){

}

void TimedObject::doSomething(){
	if(!alive())	return;
	m_lifetime--;
	realDoSomething();	//implemented in derived classes
	if(m_lifetime == 0){
		lastAction();	//implemented in derived sprayer
		setDead();
	}
}

void TimedObject::changeLifetime(int newlifetime){
	m_lifetime = newlifetime;
}


////////////////////////
//BUG SPRAYER FUNCTIONS
////////////////////////
BugSprayer::BugSprayer(StudentWorld* swptr)
	:TimedObject(swptr, IID_BUGSPRAYER, swptr->getPlayer()->getX(), 
	swptr->getPlayer()->getY(), 40){
}

BugSprayer::~BugSprayer(){
}

void BugSprayer::realDoSomething(){
	if(getWorld()->content(getX(), getY()) == "bug spray"){
		lastAction();
	}
}

void BugSprayer::lastAction(){
	//releases spray
	getWorld()->drop("bug spray", getX(), getY());
	std::string what;
	//right
	for(int i=1; i<=2; i++){
		what = getWorld()->content(getX()+i, getY());
		if(what != "permanent brick")
			getWorld()->drop("bug spray", getX()+i, getY());
		if(what == "permanent brick" || what == "destroyable brick")
			break;
	}
	//left
	for(int i=1; i<=2; i++){
		what = getWorld()->content(getX()-i, getY());
		if(what != "permanent brick")
			getWorld()->drop("bug spray", getX()-i, getY());
		if(what == "permanent brick" || what == "destroyable brick")
			break;
	}
	//up
	for(int i=1; i<=2; i++){
		what = getWorld()->content(getX(), getY()+i);
		if(what != "permanent brick")
			getWorld()->drop("bug spray", getX(), getY()+i);
		if(what == "permanent brick" || what == "destroyable brick")
			break;
	}
	//down
	for(int i=1; i<=2; i++){
		what = getWorld()->content(getX(), getY()-i);
		if(what != "permanent brick")
			getWorld()->drop("bug spray", getX(), getY()-i);
		if(what == "permanent brick" || what == "destroyable brick")
			break;
	}
	//play sound
	getWorld()->playSound(SOUND_SPRAY);
	getWorld()->getPlayer()->decCurNumSprayers();
	setDead();
}

////////////////////////
//BUG SPRAY FUNCTIONS
////////////////////////
BugSpray::BugSpray(StudentWorld *swptr, int startX, int startY)
	:TimedObject(swptr, IID_BUGSPRAY, startX, startY, 3){
}

BugSpray::~BugSpray(){
}

void BugSpray::realDoSomething(){
	//check player's location
	if(getWorld()->sameSquareAsPlayer(getX(), getY())){
		getWorld()->killPlayer();
		getWorld()->playSound(SOUND_PLAYER_DIE);
		getWorld()->decLives();
	}
}

void BugSpray::lastAction(){
	getWorld()->erase4maze(getX(), getY());		//clear its spot on the 2D maze gird!
}


////////////////////////
//GOODIE FUNCTIONS
////////////////////////
Goodie::Goodie(StudentWorld* swptr, int imageID, int startX, int startY, int lifetime)
	:TimedObject(swptr, imageID, startX, startY, lifetime)
{}

Goodie::~Goodie(){
}

void Goodie::realDoSomething(){
	if(getWorld()->sameSquareAsPlayer(getX(), getY())){
		//let student world know
		effect();	//implemented in 3 goodies
		getWorld()->increaseScore(1000);
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
	}
}

void Goodie::lastAction(){
	//doesn't do much
}


////////////////////////
//EXTRA LIFE GOODIE FUNCTIONS
////////////////////////
ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld* swptr, int startX, int startY)
	:Goodie(swptr, IID_EXTRA_LIFE_GOODIE, startX, startY, 10000){	
	changeLifetime(getWorld()->getGoodieLifetimeInTicks());
}

ExtraLifeGoodie::~ExtraLifeGoodie(){
}

void ExtraLifeGoodie::effect(){
	getWorld()->incLives();
}


////////////////////////
//WALK THRU GOODIE FUNCTIONS
////////////////////////
WalkThruGoodie::WalkThruGoodie(StudentWorld* swptr, int startX, int startY)
	:Goodie(swptr, IID_WALK_THRU_GOODIE, startX, startY, 10000){
	changeLifetime(getWorld()->getGoodieLifetimeInTicks());
}

WalkThruGoodie::~WalkThruGoodie(){
}

void WalkThruGoodie::effect(){
	getWorld()->getPlayer()->gotWalkThruGoodie();
}


////////////////////////
//INCREASE SS GOODIE FUNCTIONS
////////////////////////
IncreaseSSGoodie::IncreaseSSGoodie(StudentWorld* swptr, int startX, int startY)
	:Goodie(swptr, IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE, startX, startY, 10000){
		changeLifetime(getWorld()->getGoodieLifetimeInTicks());
}

IncreaseSSGoodie::~IncreaseSSGoodie(){
}

void IncreaseSSGoodie::effect(){
	getWorld()->getPlayer()->gotIncreaseSSGodie();

}


////////////////////////
//ZUMI FUNCTIONS
////////////////////////

Zumi::Zumi(StudentWorld *swptr, int imageID, int startX, int startY)
	:Actor(swptr, imageID, startX, startY){
		m_curDir = std::rand() % 4;
		//0:right	1:up	2:left	3:down
}

Zumi::~Zumi(){
}

void Zumi::doSomething(){
	if(!alive())	return;
	if(getWorld()->sameSquareAsPlayer(getX(), getY())){
		getWorld()->killPlayer();
		getWorld()->playSound(SOUND_PLAYER_DIE);
		getWorld()->decLives();
	}
	if(getWorld()->content(getX(), getY()) == "bug spray"){
		setDead();
		getWorld()->playSound(SOUND_ENEMY_DIE);
		add2Score();
		getWorld()->decNumZumi();

		int rand = std::rand() % 100;
		if(rand < getWorld()->getProbOfGoodieOverall())
		{
			int prob1 = getWorld()->getProbOfExtraLifeGoodie();
			int prob2 = getWorld()->getProbOfMoreSprayersGoodie();
			int prob3 = getWorld()->getProbOfWalkThruGoodie();

			int rand2 = std::rand() % 100;
			if(rand2 < prob1)
				getWorld()->drop("extra life goodie", getX(), getY());
			if(rand2 >= prob1 && rand2 < prob1+prob2)
				getWorld()->drop("increase ss goodie", getX(), getY());
			if(rand2 >= prob1+prob2 && rand2 < prob1+prob2+prob3)
				getWorld()->drop("walk thru goodie", getX(), getY());						
		}
	}

	if(m_tickCount == m_ticksPerMove){	//move only once every N ticks
		if(isInSmellDistance(getX(), getY())){
			int dir;
			if(isReachable(getX(), getY(), dir)){	//this will return true only when its' ComplexZumi and ther exist a path to the player
				complexZumiMove(dir);
				if(m_tickCount > 0)	
					m_tickCount--;
				else if(m_tickCount == 0)	
					m_tickCount = m_ticksPerMove;
				return;
			}
		}		
		ZumiMove();	//regular zumi move
	}
	if(m_tickCount > 0)	
		m_tickCount--;
	else if(m_tickCount == 0)	
		m_tickCount = m_ticksPerMove;
	
}


void Zumi::ZumiMove(){
				//0:right	1:up	2:left	3:down
		std::string what;
		switch(curDir())
		{
		case 0:	//right
			what = getWorld()->content(getX()+1, getY());
			if(what == "permanent brick" || what == "destroyable brick"
				|| what == "bug sprayer"){
				newDir();
				return;
			}
			moveTo(getX()+1, getY());
			break;

		case 1:	//up
			what = getWorld()->content(getX(), getY()+1);
			if(what == "permanent brick" || what == "destroyable brick"
				|| what == "bug sprayer"){
				newDir();
				return;
			}
			moveTo(getX(), getY()+1);
			break;

		case 2:	//left
			what = getWorld()->content(getX()-1, getY());
			if(what == "permanent brick" || what == "destroyable brick"
				|| what == "bug sprayer"){
				newDir();
				return;
			}
			moveTo(getX()-1, getY());
			break;

		case 3:	//down
			what = getWorld()->content(getX(), getY()-1);
			if(what == "permanent brick" || what == "destroyable brick"
				|| what == "bug sprayer"){
				newDir();
				return;
			}
			moveTo(getX(), getY()-1);
			break;

		default:
			break;
	}
}

void Zumi::assignMoveTicks(int ticksPerMove){
	m_ticksPerMove = ticksPerMove;
}

void Zumi::assignTickCounts(int tickCount){
	m_tickCount = tickCount;
}

void Zumi::newDir(){	//in ZumiMove, call newDir and return immediately
	m_curDir = std::rand() % 4;
}

int Zumi::curDir(){
	return m_curDir;
}



////////////////////////
//SIMPLE ZUMI FUNCTIONS
////////////////////////
SimpleZumi::SimpleZumi(StudentWorld *swptr, int startX, int startY)
	:Zumi(swptr, IID_SIMPLE_ZUMI, startX, startY)
{
	assignMoveTicks(getWorld()->getTicksPerSimpleZumiMove());
	assignTickCounts(getWorld()->getTicksPerSimpleZumiMove());
}

SimpleZumi::~SimpleZumi(){
}

void SimpleZumi::add2Score(){
	getWorld()->increaseScore(100);
}

bool SimpleZumi::isInSmellDistance(int x, int y){
	return false;	//this doesn't apply to simple zumi
}

bool SimpleZumi::isReachable(int x, int y, int &dir){
	return false;
}

void SimpleZumi::complexZumiMove(int dir){
	return;	//this doesn't apply to simple zumi
}



////////////////////////
//COMPLEX ZUMI FUNCTIONS
////////////////////////

ComplexZumi::ComplexZumi(StudentWorld *swptr, int startX, int startY)
	:Zumi(swptr, IID_COMPLEX_ZUMI, startX, startY)
{
	assignMoveTicks(getWorld()->getTicksPerComplexZumiMove());
	assignTickCounts(getWorld()->getTicksPerComplexZumiMove());
	m_smellD = getWorld()->getComplexZumiSearchDistance();
}

ComplexZumi::~ComplexZumi(){
}

void ComplexZumi::add2Score(){
	getWorld()->increaseScore(500);
}

bool ComplexZumi::isInSmellDistance(int x, int y){
	int horizD = abs(getX() - (getWorld()->getPlayer()->getX()));
	int verticalD = abs(getY() - (getWorld()->getPlayer()->getY()));
	
	if(horizD <= m_smellD && verticalD <= m_smellD)
		return true;
	else
		return false;
}

bool ComplexZumi::isReachable(int x, int y, int &dir){
	char maze[VIEW_HEIGHT][VIEW_WIDTH];
	std::string what;
	//initialize the 2D maze grid
	for(int i=0; i<VIEW_HEIGHT; i++){
		for(int j=0; j<VIEW_WIDTH; j++){
			what = getWorld()->content(j, VIEW_HEIGHT-i-1);
			if(what == "permanent brick" || what == "destroyable brick"
				|| what == "bug sprayer")
				maze[i][j] = 'X';
			else
				maze[i][j] = '.';
		}
	}//initialized

	//find whether there's a path to the player, and if so, 
	//get the direction of the first move that will lead to the player.
	std::queue<Coord> cs;
	cs.push(Coord(VIEW_HEIGHT-getY()-1, getX(),0 ));
	bool first = true;	//this helps keep track of the very first move's direction only
	maze[VIEW_HEIGHT-getY()-1][getX()] = 'D';	//marked as Discovered

	while(!cs.empty())
	{
		Coord cur = cs.front();
		cs.pop();
		if(cur.r() == (VIEW_HEIGHT - getWorld()->getPlayer()->getY() -1) 
			&& cur.c() == getWorld()->getPlayer()->getX()){
			dir = cur.getDir();		//now dir has the direction of the first move
			return true;			//solved the maze
		}

		//north 1
		if(maze[cur.r()-1][cur.c()] != 'X' && maze[cur.r()-1][cur.c()] != 'D')
		{
			if(first){
				cs.push(Coord(cur.r()-1, cur.c(),1));
				maze[cur.r()-1][cur.c()] = 'D';
			}
			else{
				cs.push(Coord(cur.r()-1, cur.c(), cur.getDir()));
				maze[cur.r()-1][cur.c()] = 'D';
			}
		}
		//east 2
		if(maze[cur.r()][cur.c()+1] != 'X' && maze[cur.r()][cur.c()+1] != 'D')
		{
			if(first){
				cs.push(Coord(cur.r(), cur.c()+1, 2));
				maze[cur.r()][cur.c()+1] = 'D';
			}
			else{
				cs.push(Coord(cur.r(), cur.c()+1, cur.getDir()));
				maze[cur.r()][cur.c()+1] = 'D';
			}
		}
		//south 3
		if(maze[cur.r()+1][cur.c()] != 'X' && maze[cur.r()+1][cur.c()] != 'D')
		{
			if(first){
				cs.push(Coord(cur.r()+1, cur.c(), 3));
				maze[cur.r()+1][cur.c()] = 'D';
			}
			else{
				cs.push(Coord(cur.r()+1, cur.c(), cur.getDir()));
				maze[cur.r()+1][cur.c()] = 'D';
			}
		}
		//west 4
		if(maze[cur.r()][cur.c()-1] != 'X' && maze[cur.r()][cur.c()-1] != 'D')
		{
			if(first){
				cs.push(Coord(cur.r(), cur.c()-1, 4));
				maze[cur.r()][cur.c()-1] = 'D';
			}
			else{
				cs.push(Coord(cur.r(), cur.c()-1, cur.getDir()));
				maze[cur.r()][cur.c()-1] = 'D';
			}
		}
		first = false;
	}
	return false;
}


void ComplexZumi::complexZumiMove(int dir){		
	//move in the direction passed in which is guaranteed to lead to the player in the shortest path)
	//1north 2east 3south 4west
	switch(dir)
	{
	case 1:
		moveTo(getX(), getY()+1);
		break;
	case 2:
		moveTo(getX()+1, getY());
		break;
	case 3:
		moveTo(getX(), getY()-1);
		break;
	case 4:
		moveTo(getX()-1, getY());
		break;
	default:
		break;
	}
}


