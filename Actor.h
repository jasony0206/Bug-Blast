#ifndef ACTOR_H_
#define ACTOR_H_

//2/20 12:30
#include "GameWorld.h"
#include "GraphObject.h"
#include "GameConstants.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <queue>

class StudentWorld;
//can't include studentworld : circular dependency

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor: public GraphObject	//ABC
{
public:
	Actor(StudentWorld *swptr, int imageID, int startX, int startY);
	virtual ~Actor();
	virtual void doSomething() = 0;

	StudentWorld* getWorld();
	void setDead();
	bool alive();
	
private:
	StudentWorld *m_swp;
	bool m_alive;
};

class Player: public Actor
{
public:
	Player(StudentWorld* swptr, int startX, int startY);
	virtual ~Player();
	virtual void doSomething();

	void decCurNumSprayers();
	void incMaxSprays();
	void resetMaxSprays();
	void gotWalkThruGoodie();
	void gotIncreaseSSGodie();

private:
	int m_numSprayers;	//current num of sprayers in the maze
	int m_maxSprayers;	//max cap 
	int m_WalkThruGoodieCount;
	int m_IncreaseSSGoodieCount;
};

class Brick: public Actor	//ABC
{
public:
	Brick(StudentWorld* swptr, int imageID, int startX, int startY);
	virtual ~Brick();
};

class PermaBrick: public Brick
{
public:
	PermaBrick(StudentWorld* swptr, int startX, int startY);
	virtual ~PermaBrick();
	virtual void doSomething();
};

class DestBrick: public Brick
{
public:
	DestBrick(StudentWorld* swptr, int startX, int startY);
	virtual ~DestBrick();
	virtual void doSomething();
};

class Exit: public Actor
{
public:
	Exit(StudentWorld* swptr, int startX, int startY);
	virtual ~Exit();
	virtual void doSomething();
private:
	bool exitVisible;
};

class TimedObject: public Actor		//ABC
{
public:
	TimedObject(StudentWorld* swptr, int imageID, int startX, int startY, int lifetime);
	virtual ~TimedObject();
	virtual void doSomething();			//dec lifetime, setDead if done
	virtual void realDoSomething() = 0;	//actual action by timed objects
	virtual void lastAction() = 0;		//sprayer's last action!
	void changeLifetime(int newlifetime);
private:
	int m_lifetime;
};


class BugSprayer: public TimedObject
{
public:
	BugSprayer(StudentWorld* swptr);
	virtual ~BugSprayer();
	virtual void realDoSomething();
	virtual void lastAction();
};

class BugSpray: public TimedObject
{
public:
	BugSpray(StudentWorld *swptr, int startX, int startY);
	virtual ~BugSpray();
	virtual void realDoSomething();
	virtual void lastAction();
};


class Goodie: public TimedObject	//ABC
{
public:
	Goodie(StudentWorld* swptr, int imageID, int startX, int startY, int lifetime);
	virtual ~Goodie();
	virtual void realDoSomething();
	virtual void lastAction();	//doesn't do much
	virtual void effect() = 0;
};

class ExtraLifeGoodie: public Goodie	
{
public:
	ExtraLifeGoodie(StudentWorld* swptr, int startX, int startY);
	virtual ~ExtraLifeGoodie();
	virtual void effect();
};

class WalkThruGoodie: public Goodie
{
public:
	WalkThruGoodie(StudentWorld* swptr, int startX, int startY);
	virtual ~WalkThruGoodie();
	virtual void effect();
};

class IncreaseSSGoodie: public Goodie
{
public:
	IncreaseSSGoodie(StudentWorld* swptr, int startX, int startY);
	virtual ~IncreaseSSGoodie();
	virtual void effect();
};

class Zumi: public Actor	//ABC
{
public:
	Zumi(StudentWorld *swptr, int imageID, int startX, int startY);
	virtual ~Zumi();
	virtual void doSomething();
	void ZumiMove();
	void assignMoveTicks(int ticksPerMove);
	void assignTickCounts(int tickCount);
	void newDir();
	virtual void add2Score() = 0;
	int curDir();
	virtual bool isInSmellDistance(int x, int y) = 0;
	virtual bool isReachable(int x, int y, int &dir) = 0;
	virtual void complexZumiMove(int dir) = 0;

private:
	int m_curDir;
	int m_tickCount;
	int m_ticksPerMove;
};

class SimpleZumi: public Zumi
{
public:
	SimpleZumi(StudentWorld *swptr, int startX, int startY);
	virtual ~SimpleZumi();
	virtual void add2Score();
	virtual bool isInSmellDistance(int x, int y);
	virtual bool isReachable(int x, int y, int &dir);
	virtual void complexZumiMove(int dir);
};


class ComplexZumi: public Zumi
{
public:
	ComplexZumi(StudentWorld *swptr, int startX, int startY);
	virtual ~ComplexZumi();
	virtual void add2Score();
	virtual bool isInSmellDistance(int x, int y);
	virtual bool isReachable(int x, int y, int &dir);
	virtual void complexZumiMove(int dir);

private:
	int m_smellD;
};


#endif // ACTOR_H_
