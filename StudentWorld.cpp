#include "StudentWorld.h"

GameWorld* createStudentWorld()
{
	return new StudentWorld();
}

StudentWorld::StudentWorld(){
	std::srand(std::time(0));	
}

StudentWorld::~StudentWorld(){
	delete m_player;
	for(int i=0; i<m_actorV.size(); i++)
		delete m_actorV[i];
	int a = m_actorV.size();
	for(int i=0; i<a; i++){
		m_actorV.pop_back();
	}
}

void StudentWorld::cleanUp(){	
	for(int i=0; i<VIEW_HEIGHT; i++){
		for(int j=0; j<VIEW_WIDTH; j++)
			m_maze[i][j] = "";
	}
	delete m_player;
	
	for(int i=0; i<m_actorV.size(); i++)
		delete m_actorV[i];
	//now they're all dangling ptrs
	int a = m_actorV.size();
	for(int i=0; i<a; i++){
		m_actorV.pop_back();
	}
}

unsigned int StudentWorld::getProbOfGoodieOverall() const{
	return m_ProbOfGoodieOverall;}
unsigned int StudentWorld::getProbOfExtraLifeGoodie() const{
	return m_ProbOfExtraLifeGoodie;}
unsigned int StudentWorld::getProbOfWalkThruGoodie() const{
	return m_ProbOfWalkThruGoodie;}
unsigned int StudentWorld::getProbOfMoreSprayersGoodie() const{
	return m_ProbOfMoreSprayersGoodie;}
unsigned int StudentWorld::getTicksPerSimpleZumiMove() const{
	return m_TicksPerSimpleZumiMove;}
unsigned int StudentWorld::getTicksPerComplexZumiMove() const{
	return m_TicksPerComplexZumiMove;}
unsigned int StudentWorld::getGoodieLifetimeInTicks() const{
	return m_GoodieLifetimeInTicks;}
unsigned int StudentWorld::getComplexZumiSearchDistance() const{
	return m_ComplexZumiSearchDistance;}
unsigned int StudentWorld::getBoostedSprayerLifetimeTicks() const{
	return m_BoostedSprayerLifetimeTicks;}
unsigned int StudentWorld::getMaxBoostedSprayers() const{
	return m_MaxBoostedSprayers;}
unsigned int StudentWorld::getWalkThruGoodieLifetimeTicks() const{
	return m_WalkThruGoodieLifetimeTicks;}


std::string StudentWorld::content(int a, int b){
	return m_maze[VIEW_HEIGHT-b-1][a];
}

void StudentWorld::erase4maze(int curRow, int curCol){
	m_maze[VIEW_HEIGHT-curCol-1][curRow] = "";	//updates the maze grid
}

void StudentWorld::insert2maze(std::string actor, int newRow, int newCol){
	m_maze[VIEW_HEIGHT-newCol-1][newRow] = actor;	//updates the maze grid
}

void StudentWorld::drop(std::string actor, int x, int y){
	Actor* temp;
	if(actor == "bug sprayer"){
		temp = new BugSprayer(this);
		m_actorV.push_back(temp);		
		m_maze[VIEW_HEIGHT-y-1][x] = "bug sprayer";
	}

	if(actor == "bug spray"){
		temp = new BugSpray(this, x, y);
		m_actorV.push_back(temp);
		m_maze[VIEW_HEIGHT-y-1][x] = "bug spray";
	}

	if(actor == "extra life goodie"){
		temp = new ExtraLifeGoodie(this, x, y);
		m_actorV.push_back(temp);
	}

	if(actor == "increase ss goodie"){
		temp = new IncreaseSSGoodie(this, x, y);
		m_actorV.push_back(temp);
	}

	if(actor == "walk thru goodie"){
		temp = new WalkThruGoodie(this, x, y);
		m_actorV.push_back(temp);
	}
}

int StudentWorld::numZumi(){
	return m_numZumi;
}

void StudentWorld::decNumZumi(){
	m_numZumi--;
}

bool StudentWorld::clear(){
	return m_clear;
}

void StudentWorld::setClear(){
	m_clear = true;
}


bool StudentWorld::sameSquareAsPlayer(int curx, int cury) const{
	if(m_player->getX() == curx && m_player->getY() == cury)
		return true;
	return false;
}

bool StudentWorld::complete(){
	return m_complete;
}

void StudentWorld::setComplete(){
	m_complete = true;
}

void StudentWorld::setDisplayText(){
	int score = getScore();
	int level = getLevel();
	int livesLeft = getLives();

	std::ostringstream oss;
	oss.setf(std::ios::fixed);
	oss.precision(0);
	oss << "Score: " << std::setw(8) << score << "  Level: " 
		<< std::setw(2) << level << "  Lives: " << std::setw(3) 
		<< livesLeft << "  Bonus: " << std::setw(6) <<m_bonus;

	std::string s = oss.str();
	setGameStatText(s);
}

int StudentWorld::init()
{
	int curLvl = getLevel(); 		
	m_clear = false;
	m_complete = false;	
	m_numZumi = 0;
	//load a level
	Level lev;
	Level::LoadResult result;	
	std::string lvlFileName = "level0";	
	switch(curLvl)
	{
	//level 0		
	case 0: 
		result = lev.loadLevel("level00.dat");
		if (result == Level::load_fail_file_not_found){
			std::cout << "Could not find level00.dat data file\n";
			return GWSTATUS_NO_FIRST_LEVEL;
		}
		else if (result == Level::load_fail_bad_format){ 
			std::cout << "Your level was improperly formatted\n"; 
			return GWSTATUS_LEVEL_ERROR;
		}
		else if (result == Level::load_success)
			std::cout << "Successfully loaded level00.dat\n"; 
		break;
		
	//level not 0
	default:
		lvlFileName += (curLvl+48);
		lvlFileName += ".dat";
		result = lev.loadLevel(lvlFileName);
		if (result == Level::load_fail_file_not_found){
			std::cout << "Could not find " << lvlFileName << " data file\n";
			return GWSTATUS_PLAYER_WON;
		}
		else if (result == Level::load_fail_bad_format){
			std::cout << "Your level was improperly formatted\n"; 
			return GWSTATUS_LEVEL_ERROR;
		}
		else if (result == Level::load_success)
			std::cout << "Successfully loaded " <<  lvlFileName << std::endl; 
		break;
	}
	//level loaded

	//get level option values
	m_ProbOfGoodieOverall = lev.getOptionValue(optionProbOfGoodieOverall);
	m_ProbOfExtraLifeGoodie = lev.getOptionValue(optionProbOfExtraLifeGoodie);
	m_ProbOfWalkThruGoodie = lev.getOptionValue(optionProbOfWalkThruGoodie);
	m_ProbOfMoreSprayersGoodie = lev.getOptionValue(optionProbOfMoreSprayersGoodie);
	m_TicksPerSimpleZumiMove = lev.getOptionValue(optionTicksPerSimpleZumiMove);
	m_TicksPerComplexZumiMove = lev.getOptionValue(optionTicksPerComplexZumiMove);
	m_GoodieLifetimeInTicks = lev.getOptionValue(optionGoodieLifetimeInTicks);
	m_ComplexZumiSearchDistance = lev.getOptionValue(optionComplexZumiSearchDistance);
	m_BoostedSprayerLifetimeTicks = lev.getOptionValue(optionBoostedSprayerLifetimeTicks);
	m_MaxBoostedSprayers = lev.getOptionValue(optionMaxBoostedSprayers);
	m_WalkThruGoodieLifetimeTicks = lev.getOptionValue(optionWalkThruLifetimeTicks);

	m_bonus = lev.getOptionValue(optionLevelBonus);

	//allocate actors by traversing level's m_maze
	Level::MazeEntry item;
	for(int i=0; i<VIEW_HEIGHT; i++)
	{
		for(int j=0; j<VIEW_WIDTH; j++)
		{
			item = lev.getContentsOf(j, VIEW_HEIGHT-i-1);
			Actor* temp;
			switch(item)
			{
			case Level::player:
				m_player = new Player(this, j, VIEW_HEIGHT-i-1);				
				break;
			case Level::perma_brick:
				temp = new PermaBrick(this, j, VIEW_HEIGHT-i-1);
				m_actorV.push_back(temp);
				m_maze[i][j] = "permanent brick";
				break;
			case Level::destroyable_brick:
				temp = new DestBrick(this, j, VIEW_HEIGHT-i-1);
				m_actorV.push_back(temp);
				m_maze[i][j] = "destroyable brick";
				break;
			case Level::exit:
				temp = new Exit(this, j, VIEW_HEIGHT-i-1);
				m_actorV.push_back(temp);
				break;
			case Level::simple_zumi:
				temp = new SimpleZumi(this, j, VIEW_HEIGHT-i-1);
				m_actorV.push_back(temp);
				m_numZumi++;
				break;				
			case Level::complex_zumi:
				temp = new ComplexZumi(this, j, VIEW_HEIGHT-i-1);
				m_actorV.push_back(temp);
				m_numZumi++;
				break;
				
			default:
				break;
			}
		}
	}

		
	return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move()
{
	setDisplayText();
	m_player->doSomething();
	for(int i=0; i<m_actorV.size(); i++){
		m_actorV[i]->doSomething();
	
		if(!(m_player->alive()))
			return GWSTATUS_PLAYER_DIED;

		//if player completed current level, increase score by bonus and return GWSTATUS
		if(m_complete){
			increaseScore(m_bonus);
			return GWSTATUS_FINISHED_LEVEL;
		}
	}

	for(int i=0; i<m_actorV.size(); i++){
		if(!(m_actorV[i]->alive())){
			delete m_actorV[i];
			if(i == m_actorV.size() -1)	//if the last actor ptr
				m_actorV.pop_back();	//remove the item
			else{
				//make the dangling ptr point to the last item
				m_actorV[i] = m_actorV[m_actorV.size() -1];
				m_actorV.pop_back();	//remove the last item
			}
		}
		//numZumi is taken care by actors
	}

	if(m_bonus > 0)	m_bonus--;
	
	if(m_numZumi == 0)	setClear();

	if(!(m_player->alive()))
		return GWSTATUS_PLAYER_DIED;

	if(m_complete){
		increaseScore(m_bonus);
		return GWSTATUS_FINISHED_LEVEL;
	}
	return GWSTATUS_CONTINUE_GAME;
}


// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
