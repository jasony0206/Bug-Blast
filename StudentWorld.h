#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <vector>
#include "Level.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	virtual int init();
	virtual int move();
	virtual void cleanUp();

	StudentWorld();
	virtual ~StudentWorld();
	std::string content(int a, int b);
	void erase4maze(int curRow, int curCol);
	void insert2maze(std::string actor, int newRow, int newCol);
	void drop(std::string actor, int x, int y);

	int numZumi();
	void decNumZumi();
	Player* getPlayer() const{return m_player;}
	void killPlayer(){m_player->setDead();}

	bool clear();
	void setClear();
	
	bool sameSquareAsPlayer(int curx, int cury) const;
	bool complete();
	void setComplete();
	void setDisplayText();

	unsigned int getProbOfGoodieOverall() const;
    unsigned int getProbOfExtraLifeGoodie() const;
    unsigned int getProbOfWalkThruGoodie() const;
    unsigned int getProbOfMoreSprayersGoodie() const;
    unsigned int getTicksPerSimpleZumiMove() const;
    unsigned int getTicksPerComplexZumiMove() const;
    unsigned int getGoodieLifetimeInTicks() const;
    unsigned int getComplexZumiSearchDistance() const;
	unsigned int getBoostedSprayerLifetimeTicks() const;
	unsigned int getMaxBoostedSprayers() const;
	unsigned int getWalkThruGoodieLifetimeTicks() const;

private:
	Player *m_player;
	std::vector<Actor*> m_actorV;
	std::string m_maze[VIEW_HEIGHT][VIEW_WIDTH];
	int m_bonus;	//reset in init
	int m_numZumi;	//reset in init
	bool m_clear;	//turns true when all zumi killed
	bool m_complete;//true when player steps on revealed exit, for GWSTATUS

	int m_ProbOfGoodieOverall;
	int m_ProbOfExtraLifeGoodie;
	int m_ProbOfWalkThruGoodie;
	int m_ProbOfMoreSprayersGoodie;
	int m_TicksPerSimpleZumiMove;
	int m_TicksPerComplexZumiMove;
	int m_GoodieLifetimeInTicks;
	int m_ComplexZumiSearchDistance;
	int m_BoostedSprayerLifetimeTicks;
	int m_MaxBoostedSprayers;
	int m_WalkThruGoodieLifetimeTicks;
};

#endif // STUDENTWORLD_H_
