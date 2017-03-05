#include "Actor.h"
#include "StudentWorld.h"
#include "Field.h"
#include "Compiler.h"
#include <iostream>
using namespace std;
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
/*
Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir = none) : GraphObject(imageID, startX, startY, dir) {
	setVisible(true);
	m_isAlive = true;
	m_world = world;
};
*/
Actor::Actor(
	StudentWorld* world,
	int imageID,
	int startX,
	int startY,
	Direction dir,
	int depth,
	double size,
	int hit,
	bool Move,
	bool block) :
	GraphObject(
		imageID, 
		startX, 
		startY, 
		dir, 
		depth, 
		size), m_hitPoint(hit), m_sw(world), m_canmove(Move), m_isblock(block), m_stunnedTicks(0)
{
	m_ate = false;
}

Pebble::Pebble(StudentWorld*world, int startX, int startY)
	: Actor(world, IID_ROCK, startX, startY, right, 1,0.25, 10000, false, true )
{}
Food::Food(StudentWorld*world, int startX, int startY, int hp)
	: Actor(world,IID_FOOD, startX, startY, right, 2, 0.25, hp, false, false)
{}

Water::Water(StudentWorld*world, int startX, int startY)
	: Actor(world, IID_WATER_POOL, startX, startY, right, 2, 0.25, 10000, false, false)
{}

Poison::Poison(StudentWorld*world, int startX, int startY)
	: Actor(world, IID_POISON, startX, startY, right, 2, 0.25, 10000, false, false)
{}

Anthill::Anthill(StudentWorld* world, int startX, int startY, int cn, Compiler* c)
	: Actor(world, IID_ANT_HILL, startX, startY, right, 2, 0.25, 8999, false, false), m_colonyNum(cn)
{
	m_antNumber = 0;
	m_compiler = c;
	m_foodDropped = 0;
	//m_queenEnergy = 0;
}
Pheromone::Pheromone(StudentWorld* world, int startX, int startY, int iid)
	: Actor(world, iid, startX, startY, right, 2, 0.25, 256, false, false)
{
}

void Pheromone::doSomething() 
{
	consumeEnergy();
	if (getHitPoint() <= 0) setDead(true);
}
void Anthill::giveBirth()
{
	StudentWorld* w = getWorld();
	int iid;
	Compiler* cpl;
	switch (m_colonyNum)
	{
	case 0:	
		iid = IID_ANT_TYPE0; cpl = w->getCompilerForColony(0); break;
	case 1:	
		iid = IID_ANT_TYPE1; cpl = w->getCompilerForColony(1); break;
	case 2:	
		iid = IID_ANT_TYPE2; cpl = w->getCompilerForColony(2); break;
	case 3:	
		iid = IID_ANT_TYPE3; cpl = w->getCompilerForColony(3); break;
	default:
		iid = IID_ANT_TYPE0; cpl = nullptr;break;
	}
	Ant* newAnt = new Ant(w, getX(), getY(), iid, this, cpl);
	w->addActor(getX(), getY(), newAnt);
	m_antNumber++;

}
void Anthill::queenEatFood()
{
	int amt = (m_foodDropped <= 10000)
		? m_foodDropped : 10000;
	setHitPoint(getHitPoint () + amt);
	m_foodDropped -= amt;
}
void Anthill::doSomething()  
{
	consumeEnergy();
	//cout << "HP: "<< getHitPoint() << endl;
	if (getHitPoint() <= 0)
	{
		setDead(true); return;
	}
	if (m_foodDropped>0)
	{
		queenEatFood(); return;
	}
	//cout << getHitPoint() << endl;
	if (getHitPoint()>=2000) //give birth to an new ant
	{
		giveBirth();
		setHitPoint( getHitPoint()- 1500);
	}
	//cout << "Queen: "<<m_queenEnergy << endl;
	//cout << "Anthill doSomething is called." << endl;
}

Ant::Ant(StudentWorld* world, int startX, int startY, int imageid, Anthill* hill, Compiler* cpler)
	: Actor(world, imageid, startX, startY, Direction(randInt(1, 4)), 1, 0.25, 1500, true, false)
{
	m_hillPointer = hill;
	m_compilerPointer = cpler;
	
	setWasBit(false);
	setWasBlockedFromMoving(false);

	m_foodCarried = 0; //1800 at most
}

void Ant::doSomething()
{
	consumeEnergy();                 
	if (getHitPoint() <= 0)
	{
		//int antnum = m_hillPointer->getAntNumber();
		//m_hillPointer->setAntNumber(antnum - 1);
		setDeadToFood();	return;		
	}
	
	if (getStunnedTicks()>0)
	{
		checkStun(); return;      
	}
	
	int c_counter = 0;
	while (c_counter <= 10)
	{
		runCommand();      
		c_counter++;
	}
	
}

Babygrasshopper::Babygrasshopper(StudentWorld*world, int startX, int startY, int iid = IID_BABY_GRASSHOPPER, int hp = 500)
	:Actor(world,iid, startX, startY, right, 1, 0.25, hp, true, false)
{
	setDistance(randInt(2, 10));
	setStunnedTicks(0);

}
Adultgrasshopper::Adultgrasshopper(StudentWorld*world, int startX, int startY, int iid=IID_ADULT_GRASSHOPPER, int hp=1600)
     :Babygrasshopper(world, startX, startY, iid, hp)
{
	setDistance(randInt(2, 10));
	setStunnedTicks(0);
}


bool Actor::moveForward()
{
	Direction dir = getDirection();
	StudentWorld* temp = getWorld();
	switch (dir)
	{
	case GraphObject::none:
		return false;
		break;

	case GraphObject::up:
		if (temp->returnID(getX(), getY() + 1) == IID_ROCK)
		{
			setDistance(0);
			setWasBlockedFromMoving(true); 
			return false;
		}
		/*
		if (temp->returnID(getX(), getY() + 1) == IID_WATER_POOL)
		{
			moveTo(getX(), getY() + 1);
			setStunned(true);
			setStunnedTicks(2);
			return ;
		}
		*/
		moveTo(getX(), getY() + 1); 
		setStunned(false); 
		//setStunnedTicks(2); 
		setAte(false);
		return true;
		break;

	case GraphObject::right:
		if (temp->returnID(getX() + 1, getY()) == IID_ROCK)
		{
			setDistance(0);
			setWasBlockedFromMoving(true);
			return false;
		}
		/*
		if (temp->returnID(getX() + 1, getY()) == IID_WATER_POOL)
		{
			moveTo(getX()+1, getY());
			setStunned(true);
			setStunnedTicks(2);
			return;
		}
		*/
		moveTo(getX()+1, getY()); 
		setStunned(false);
		//setStunnedTicks(2); 
		setAte(false);
		return true;
		break;
	case GraphObject::down:
		if (temp->returnID(getX(), getY() - 1) == IID_ROCK)
		{
			setDistance(0);
			setWasBlockedFromMoving(true);
			return false;
		}
		/*
		if (temp->returnID(getX(), getY() - 1) == IID_WATER_POOL)
		{
			moveTo(getX(), getY() - 1);
			setStunned(true);
			setStunnedTicks(2);
			return;
		}
		*/
		moveTo(getX(), getY() - 1); 
		setStunned(false); 
		//setStunnedTicks(2); 
		setAte(false);
		return true;
		break;

	case GraphObject::left:
		if (temp->returnID(getX() - 1, getY()) == IID_ROCK)
		{
			setDistance(0);
			setWasBlockedFromMoving(true);
			return false;
		}
		/*
		if (temp->returnID(getX() - 1, getY()) == IID_WATER_POOL)
		{
			moveTo(getX() - 1, getY());
			setStunned(true);
			setStunnedTicks(2);
			return;
		}
		*/
		moveTo(getX() - 1, getY()); 
		setStunned(false); 
		//setStunnedTicks(2);
		setAte(false);
		return true;
		break;
	default:
		return false;
	}
	return false;
}

bool Babygrasshopper::grasshopperEatFood()
{
	StudentWorld* w = getWorld();
	vector<Actor*> as = w->getActorsAt(getY(), getX());
	for (unsigned int i = 0; i < as.size(); i++)
	{
		if (isFood(as[i]))
		{
			int amt = as[i]->getHitPoint() > 200 ? 200 : as[i]->getHitPoint();
			setHitPoint(getHitPoint() + amt);
			setAte(true);
			as[i]->setHitPoint(as[i]->getHitPoint() - amt);
			if (as[i]->getHitPoint() <= 0) as[i]->setDead(true);
			return true;
		}
	}
	return false;
}
void Babygrasshopper::doSomething()
{
	consumeEnergy();
	if (getHitPoint() <= 0)
	{
		setDeadToFood(); return;
	}
	else if (getStunnedTicks() > 0)
	{
		checkStun(); 
		return;
	}

	if (getHitPoint() >= 1600) //turn into adult
	{
		setHitPoint(0);
		setDeadToFood();
		StudentWorld* w = getWorld();
		w->addActor(getY(), getX(), new Adultgrasshopper(w, getX(), getY()));
		//cout << "turn adult is called. " << endl;
		return;
	}
	if (getStandingOnFood()&& !getAte())
	{
		if (grasshopperEatFood())
		{
			int ran = randInt(0, 1);
			setLastRandomNumber(ran);
			if (ran == 1) //sleep by 50% chance
			{
				setStunnedTicks(2);
			}
		}
		return;
	}

	if(getDistance()==0)
	{
		int direction = randInt(1, 4);
		setLastRandomNumber(direction);
		setDistance(randInt(2, 10));
		setDirection(Direction(direction));
		//moveForward();
		return;
	}
	if(getDistance()>0) 
	{
		moveForward();
		int d = getDistance();
		setDistance(--d);
		setStunnedTicks(2);
		return;
	}
	
	if (getWasBlockedFromMoving()) //if wasblocked
	{
		setDistance(0);
		setStunnedTicks(2);
		return;
	}
}

void Actor::consumeEnergy()
{
	m_hitPoint--;
}

void Actor::setDeadToFood()
{
	if (m_hitPoint <= 0)
	{
		StudentWorld* w = getWorld();
		w->addActor(getX(), getY(), (new Food(w, getX(), getY(), 100)));
		m_dead = true;
	}
}



void Actor::setDistance(int num)
{
	distance = num;
}

int Actor::getDistance()
{
	return distance;
}

bool Actor::isAnt(Actor* a)
{
	Ant *antTest = dynamic_cast<Ant*>(a); 
	return antTest != nullptr;
}
bool Actor::isAntOfOtherColonies(Actor* a)
{
	Ant* me = static_cast<Ant*>(this);
	int myHillNum = me->getHillPointer()->getColonyNum();
	Ant* thisAnt = static_cast<Ant*>(a);
	int thisHillNum = thisAnt->getHillPointer()->getColonyNum();
	return myHillNum != thisHillNum;
}

bool Actor::isBabygrasshopper(Actor* a)
{
	Babygrasshopper *babygrasshopperTest = dynamic_cast<Babygrasshopper*>(a); 
	return babygrasshopperTest != nullptr;
}
bool Actor::isAdultgrasshopper(Actor* a)
{
	Adultgrasshopper *Test = dynamic_cast<Adultgrasshopper*>(a);
	return Test != nullptr;
}
bool Actor::isPheromone(Actor* a)
{
	Pheromone *PheromoneTest = dynamic_cast<Pheromone*>(a);
	return PheromoneTest != nullptr;
}
bool Actor::isFood(Actor* a)
{
	Food *FoodTest = dynamic_cast<Food*>(a);
	return FoodTest != nullptr;
}
bool Actor::isPoison(Actor* a)
{
	Poison *Test = dynamic_cast<Poison*>(a);
	return Test != nullptr;
}
bool Actor::isAnthill(Actor* a)
{
	Anthill *Test = dynamic_cast<Anthill*>(a);
	return Test != nullptr;
}

void Food::doSomething()
{
	//do nothing 
}

void Actor::checkStun()
{
	if (m_stunnedTicks > 0)
	{
		m_stunnedTicks--;
		/*
		if (m_stunnedTicks<=0)
		{
			m_stunned = false;
		}
		*/
	}
}

void Actor::locationInFrontOfMe(int& r, int& c)
{
	Direction d = getDirection();
	switch (d)
	{
	case GraphObject::none:
		break;
	case GraphObject::up:
		r = getX(); c = getY() + 1;
		break;
	case GraphObject::right:
		r = getX() + 1; c = getY();
		break;
	case GraphObject::down:
		r = getX(); c = getY()-1;
		break;
	case GraphObject::left:
		r = getX()-1; c = getY();
		break;
	default:
		break;
	}
}

bool Ant::isDangerInFrontOfMe()
{
	int row, col;
	locationInFrontOfMe(row, col);
	StudentWorld* world = getWorld();
	vector<Actor*> actors = world->getActorsAt(row, col);
	for (unsigned int i = 0; i < actors.size(); i++)
		if (isBabygrasshopper(actors[i]) ||
			isAdultgrasshopper(actors[i]) ||
			isPoison(actors[i]) )
			return true;
	return false;
}

bool Ant:: isPheromoneInfrontOfMe()
{
	int row, col;
	locationInFrontOfMe(row, col);
	StudentWorld* world = getWorld();
	vector<Actor*> actors = world->getActorsAt(row, col);
	for (unsigned int i = 0; i < actors.size(); i++)
		if (isPheromone(actors[i]) )
			return true;
	return false;
}



void Ant::runCommand()
{
	Compiler::Command cmd;
	if (!m_compilerPointer->getCommand(m_ic, cmd))
	{
		setDead(true);
		return;
	}
	//std::cout << m_ic << " " << cmd.opcode << " " << endl;

	switch (cmd.opcode)
	{
	case Compiler::Opcode::goto_command :
		m_ic = stoi(cmd.operand1);
		break;
	case Compiler::Opcode::if_command:
		if (
			(stoi(cmd.operand1) == Compiler::Condition::i_smell_danger_in_front_of_me
			&& isDangerInFrontOfMe())
			||
			(stoi(cmd.operand1) == Compiler::Condition::i_smell_pheromone_in_front_of_me
			&& isPheromoneInfrontOfMe())
			||
			(stoi(cmd.operand1) == Compiler::Condition::i_was_bit
			&& getWasBit())			
			||
			(stoi(cmd.operand1) == Compiler::Condition::i_am_carrying_food
			&& getCarryingFood())
			||
			(stoi(cmd.operand1) == Compiler::Condition::i_am_hungry
			&& getIAmHungry())	
			||
			(stoi(cmd.operand1) == Compiler::Condition::i_am_standing_on_my_anthill
			&& getStandingOnMyAnthill())	
			||
			(stoi(cmd.operand1) == Compiler::Condition::i_am_standing_on_food
			&& getStandingOnFood())
			||
			(stoi(cmd.operand1) == Compiler::Condition::i_am_standing_with_an_enemy
			&& getStandingWithEnemy())
			||
			(stoi(cmd.operand1) == Compiler::Condition::i_was_blocked_from_moving
			&& getWasBlockedFromMoving())
			||
			(stoi(cmd.operand1) == Compiler::Condition::last_random_number_was_zero
			&& getLastRandomNumberWasZero())
			)
			m_ic = stoi(cmd.operand2);
		else m_ic++;
		break;
		/*
		enum Condition
		{
		invalid_if = -1,
				i_smell_danger_in_front_of_me,		// 0
				i_smell_pheromone_in_front_of_me,	// 1
				i_was_bit,							// 2
				i_am_carrying_food,					// 3
				i_am_hungry,						// 4
				i_am_standing_on_my_anthill,		// 5
				i_am_standing_on_food,				// 6
				i_am_standing_with_an_enemy,		// 7
				i_was_blocked_from_moving,			// 8
				last_random_number_was_zero			// 9
		};
		*/

	case Compiler::Opcode::emitPheromone:
		emitPheromone();
		m_ic++;
		break;
	case Compiler::Opcode::faceRandomDirection:
		faceRandomDirection();
		m_ic++;
		break;
	case Compiler::Opcode::rotateClockwise:
		rotateClockwise();
		m_ic++;
		break;
	case Compiler::Opcode::moveForward:
		moveForward();
		m_ic++;
		break;
	case Compiler::Opcode::bite:
		bite();
		m_ic++;
		break;
	case Compiler::Opcode::pickupFood:
		pickupFood();
		m_ic++;
		break;
	case Compiler::Opcode::dropFood:
		dropFood();
		m_ic++;
		break;
	case Compiler::Opcode::eatFood:
		eatFood();
		m_ic++;
		break;
	case Compiler::Opcode::generateRandomNumber:
		generateRandomNumber(stoi(cmd.operand1 ));
		m_ic++;
		break;
	default:
		break;
	}

	//cout << m_ic << endl;
	/*
	enum Opcode {
		invalid = -1,
		label,
				goto_command,
				if_command,
				emitPheromone,
				faceRandomDirection,
				rotateClockwise,
				rotateCounterClockwise,
				moveForward,
				bite,
				pickupFood,
				dropFood,
				eatFood,
				generateRandomNumber
	};
	*/
}

void Ant::emitPheromone()
{
	int colonyNum = m_hillPointer->getColonyNum();
	int iid;
	switch (colonyNum)
	{
	case 0:
		iid = IID_PHEROMONE_TYPE0; break;
	case 1:
		iid = IID_PHEROMONE_TYPE1; break;
	case 2:
		iid = IID_PHEROMONE_TYPE2; break;
	case 3:
		iid = IID_PHEROMONE_TYPE3; break;
	default:
		break;
	}
	StudentWorld* w = getWorld();
	w->addActor(getY(), getX(), new Pheromone(w, getX(), getY(), iid));
}
void Ant::faceRandomDirection()
{
	setDirection(Direction(randInt(1, 4)));
}

void Ant::rotateClockwise()
{
	Direction d = getDirection();
	for (unsigned int i = 0; i < 4; i++)
	{
		setDirection(Direction(1 + ((d-1) + i) % 4));
	}
}
void Ant::rotateCounterClockwise()
{
	Direction d = getDirection();
	for (unsigned int i = 0; i < 4; i++)
	{
		setDirection(Direction(1 + ((d - 1)+4 - i) % 4));
	}
	return;
}
void Ant::bite()
{
	if (getStandingWithEnemy())
	{
		StudentWorld* w = getWorld();
		vector<Actor*> as = w->getActorsAt(getY(), getX());

		for (unsigned int i = 0; i < as.size(); i++) // just bite the first enemy
		{
			if (as[i] != nullptr)
			{
				if (isBabygrasshopper(as[i]) || isAdultgrasshopper(as[i]))
				{
					as[i]->setHitPoint(as[i]->getHitPoint() - 100);
					//if (as[i]->getHitPoint() <= 0)
						//as[i]->setDead(true);
				}
				if (isAnt(as[i]) && as[i] != this)
				{
					as[i]->setHitPoint(as[i]->getHitPoint() - 100);
					//if (as[i]->getHitPoint() <= 0)
						//as[i]->setDead(true);
					//static_cast<Ant*>(as[i])->setWasBit(true);
				}
			}
		}
	}
	return;
}
void Ant::pickupFood()
{
	if (getStandingOnFood())
	{
		StudentWorld* w = getWorld();
		vector<Actor*> as = w->getActorsAt(getY(), getX());
		for (unsigned int i = 0; i < as.size(); i++)
		{
			if (isFood(as[i]))
			{
				int amount = as[i]->getHitPoint();
				amount = (amount > 400) ? 400 : as[i]->getHitPoint();

				amount = (1800 - m_foodCarried >= amount) ?
					amount : 1800 - m_foodCarried;

				m_foodCarried += amount;
				as[i]->setHitPoint(as[i]->getHitPoint() - amount);
				if (as[i]->getHitPoint() <= 0) as[i]->setDead(true);
			}
			
		}
	}
	return;
}
void Ant::dropFood()
{
	if (getStandingOnMyAnthill())
	{
		m_hillPointer->setFoodDropped(m_foodCarried);
		m_foodCarried = 0;
		return;
	}
	StudentWorld* w = getWorld();
	vector<Actor*> as = w->getActorsAt(getY(), getX());
	for (unsigned int i = 0; i < as.size(); i++)
	{
		if (as[i]!=nullptr)
		{
			if (isFood(as[i]))
			{
				as[i]->setHitPoint(as[i]->getHitPoint() + m_foodCarried);
				m_foodCarried = 0;
				return;
			}
		}
	}
	if (m_foodCarried>0)
	{
		w->addActor(getY(),getX(), new Food(w, getX(), getY(), m_foodCarried) );
		m_foodCarried = 0;
	}

	return;
}
void Ant::eatFood()
{
	int amt = (m_foodCarried > 100) ? 100 : m_foodCarried;
	setHitPoint(getHitPoint() + amt);
	m_foodCarried -= amt;
	return;
}
void Ant::generateRandomNumber(int max)
{
	setLastRandomNumber(randInt(1, max - 1));
	return;
}

bool Ant::getStandingOnMyAnthill()
{
	int mycurrentX = getX();
	int mycurrentY = getY();
	int myHillX = m_hillPointer->getX();
	int myHillY = m_hillPointer->getY();
	if (mycurrentX == myHillX && mycurrentY == myHillY)
	{
		//cout << "I'm home." << endl;
		return true;
	}
	return false;
}

bool Actor::getStandingOnFood()
{
	StudentWorld* w = getWorld();
	vector<Actor*> actors = w->getActorsAt(getY(), getX());
	for (unsigned int i = 0; i < actors.size(); i++)
	{
		if (actors[i]!=nullptr && isFood(actors[i]))
		{
			//cout << "i'm on food" << endl;
			return true;
		}
	}
	return false;
}

bool Ant::getStandingWithEnemy()
{
	StudentWorld* w = getWorld();
	vector<Actor*> as = w->getActorsAt(getY(), getX());
	for (unsigned int i = 0; i < as.size(); i++)
	{
		if (as[i] != nullptr)
		{
			if (isBabygrasshopper(as[i]) || isAdultgrasshopper(as[i])) return true;
			if (isBabygrasshopper(as[i])) return true;
			if (isAnt(as[i]))
			{
				int myHillNum = m_hillPointer->getColonyNum();
				Ant* thisAnt = static_cast<Ant*>(as[i]);
				int thisHillNum = thisAnt->getHillPointer()->getColonyNum();	
				return myHillNum != thisHillNum;
			}
		}
	}
	return false;
}

bool Ant::getLastRandomNumberWasZero()
{
	return getLastRandomNumber() == 0;
}

void Poison::doSomething()
{
	StudentWorld* w = getWorld();
	vector<Actor*> as = w->getActorsAt(getY(), getX());
	for (unsigned int i = 0; i < as.size(); i++)
	{
		if (isAnt(as[i]) || isBabygrasshopper(as[i])||isAdultgrasshopper(as[i]))
		{
			as[i]->setHitPoint(as[i]->getHitPoint() - 150);
			/*if (as[i]->getHitPoint() <= 0)
				as[i]->setDead(true);*/

		}
	}
}
void Water::doSomething()
{
	StudentWorld* w = getWorld();
	vector<Actor*> as = w->getActorsAt(getY(), getX());
	for (unsigned int i = 0; i < as.size(); i++)
	{
		if (isAnt(as[i]) || isBabygrasshopper(as[i])||isAdultgrasshopper(as[i]))
		{
			if (!as[i]->getStunned())
			{
				as[i]->setStunned(true);
				as[i]->setStunnedTicks(as[i]->getStunnedTicks()+2);
			}
		}
	}
}


void Adultgrasshopper::doSomething()
{
	
	consumeEnergy();
	if (getHitPoint() <= 0)
	{
		setDeadToFood(); return;
	}
	if (getStunnedTicks()>0)
	{
		checkStun(); return;
	}
	
	int oneThird = randInt(1,3);
	if (oneThird == 1)  // bite
	{
		StudentWorld* w = getWorld();
		vector<Actor*> as = w->getActorsAt(getY(), getX());
		if (as.size()>1)
		{
			for (unsigned int i = 0; i < as.size(); i++)
			{
				if (as[i] != this)
				{
					if (isAnt(as[i]) || isBabygrasshopper(as[i]) || isAdultgrasshopper(as[i]))
					{
						int hp = as[i]->getHitPoint();
						as[i]->setHitPoint(hp - 50);
						setStunnedTicks(2);
						return;
					}
					
				}
			}
		}
		return;
	}
	
	int oneTenth = randInt(1, 10);//jump
	if (oneTenth==1)
	{
		
		int angle = randInt(0, 360);
		int newrow = static_cast<int>(getY() + (10 * sin(angle)));
		newrow = (newrow > (VIEW_HEIGHT - 1)) ? VIEW_HEIGHT - 1 : newrow;
		newrow = (newrow < 1) ? 1 : newrow;
		
		int newcol = static_cast<int>( getX() + (10 * cos(angle)));
		newcol = (newcol > (VIEW_WIDTH - 1)) ? VIEW_WIDTH - 1 : newcol;
		newcol = (newcol < 1) ? 1 : newcol;
		/*
		StudentWorld* w = getWorld();
		w->addActor(newrow, newcol, this);
		vector<Actor*> as = w->getActorsAt(getY(), getX());
		vector<Actor*>::iterator it = as.begin();
		
		while (it!=as.end())
		{
			if (this == *it)
			{
				as.erase(it); break;
			}
			it++;
		}
		*/
		setStunnedTicks(2);
		return;
		
	}

	if (getStandingOnFood() && !getAte())// eat
	{
		grasshopperEatFood();
		setStunnedTicks(2);
		return;
	}
	if (getDistance() > 0)
	{
		moveForward();
		int d = getDistance();
		setDistance(--d);
		setStunnedTicks(2);
		return;
	}
	else
	{
		int direction = randInt(1, 4);
		setLastRandomNumber(direction);
		setDistance(randInt(2, 10));
		setDirection(Direction(direction));
		setStunnedTicks(2);
		//moveForward();
		return;
	}
	if (getWasBlockedFromMoving()) //if wasblocked
	{
		setDistance(0);
		return;
	}
	return;
}
