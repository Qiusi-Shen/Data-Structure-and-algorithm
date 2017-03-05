#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
class StudentWorld;
class Compiler;

class Actor:public GraphObject
{
public: 
	Actor(
		StudentWorld* world,
		int imageID,
		int startX,
		int startY,
		Direction dir,
		int depth,
		double size,
		int hit,
		bool move,
		bool block
		);
	virtual void doSomething() {} 
	

	virtual ~Actor() {}

	int getHitPoint() const { return m_hitPoint; }
	void setHitPoint(int num) { m_hitPoint = num; }

	void setDeadToFood();
	bool getDead() const { return m_hitPoint<=0; }
	bool canMove() const { return m_canmove; }

	StudentWorld* getWorld() const 	{ return m_sw; }
	
	bool isAnt(Actor*);
	bool isAntOfOtherColonies(Actor*);
	bool isBabygrasshopper(Actor*);
	bool isAdultgrasshopper(Actor*);
	bool isPheromone(Actor*);
	bool isFood(Actor*);
	bool isPoison(Actor*);
	bool isAnthill(Actor*);
	
	
	void consumeEnergy();

	void setDistance(int num);
	int  getDistance();
	int  getStunnedTicks()const { return m_stunnedTicks; }
	void setStunnedTicks(int s) { m_stunnedTicks = s; }
	void setStunned(bool s) { m_stunned = s; }
	bool getStunned() { return m_stunned; }
	void setDead(bool b) { m_dead = b; }
	
	void checkStun();          
	bool moveForward();        

	void locationInFrontOfMe(int& row, int& col);

	bool getWasBit() { return m_wasBit; }
	void setWasBit(bool b) { m_wasBit = b; }
	void setLastRandomNumber(int r) { m_lastRandomNumber = r; }
	int getLastRandomNumber() { return m_lastRandomNumber; }
	bool getWasBlockedFromMoving() { return m_wasBlocked; } 
	void setWasBlockedFromMoving(bool b) { m_wasBlocked = b; }
	bool getStandingOnFood();  

	bool getAte() { return m_ate; }
	void setAte(bool b) { m_ate = b; }

private: 
	bool m_dead;
	bool m_canmove;
	bool m_isblock; // is is blocking
	int m_hitPoint;
	StudentWorld* m_sw;

	int m_stunnedTicks;
	int distance;  // continued steps
	bool m_stunned;
	bool m_wasBit;
	bool m_wasBlocked;
	int m_lastRandomNumber;
	bool m_ate; //eat once only
	//
	

};

class Pebble
	:public Actor
{
public:
	Pebble(StudentWorld*world, int startX, int startY);
	virtual void doSomething() {}
private:
};

class Food : public Actor
{
public:
	Food(StudentWorld*world, int startX, int startY, int hp =6000);
	virtual void doSomething();
private:
};

class Water : public Actor
{
public:
	Water(StudentWorld*world, int startX, int startY);
	virtual void doSomething();
private:
};

class Poison : public Actor
{
public:
	Poison(StudentWorld*world, int startX, int startY);
	virtual void doSomething();
private:
};

class Anthill
	:public Actor
{
public:
	Anthill(StudentWorld* world, int startX, int startY, int colonyNumber, Compiler* c);
	virtual void doSomething();
	void giveBirth();
	int getColonyNum() { return m_colonyNum; }
	void queenEatFood();
	int getAntNumber() { return m_antNumber; }
	void setAntNumber(int n) { m_antNumber = n; }
	void setFoodDropped(int f) { m_foodDropped += f; }
private:
	int m_antNumber;
	int m_colonyNum;
	Compiler* m_compiler;
	int m_foodDropped;
	//int m_queenEnergy;
};
class Pheromone
	:public Actor
{
public:
	Pheromone(StudentWorld* world, int startX, int startY, int iid);
	virtual void doSomething();

private:

};
class Ant : public Actor
{
public:
	Ant(StudentWorld*world, int startX, int startY, int imageId, Anthill* hill, Compiler* cpler);
	virtual void doSomething();
	Anthill* getHillPointer() { return m_hillPointer; }
	
private:
	/*
	Has 1,500 hit points.
	b. Faces in a random direction.
	c. Is not in a stunned state.
	d. Holds 0 food.
	e. Was not previously bitten.
	f. Was not previously blocked from moving.
	g. Has a last random number value of 0.
	h. Has a starting instruction counter value of 0, indicating that the first command a
	new ant must execute during its first tick is the first command in its Compiler
	object�s vector of commands.
	*/
	Anthill* m_hillPointer;
	Compiler* m_compilerPointer;
	
	int m_ic;

	// following data and function members are for Interpreter function
	//see page 35
	int m_foodCarried;


	bool isDangerInFrontOfMe();  //done
	bool isPheromoneInfrontOfMe(); //done

	bool getCarryingFood() { return m_foodCarried > 0; } //done
	bool getIAmHungry() { return getHitPoint() < 25; } //done
	bool getStandingOnMyAnthill();     //done
	bool getStandingWithEnemy();  // done
	bool getLastRandomNumberWasZero(); //done

	void runCommand(); //done
	void emitPheromone();  //done
	void faceRandomDirection(); //done
	void rotateClockwise(); //done
	void rotateCounterClockwise(); //done
	void bite();//done
	void pickupFood(); //done
	void dropFood(); //done
	void eatFood();  //done
	void generateRandomNumber(int max); // done

};

class Babygrasshopper : public Actor
{
public:
	Babygrasshopper(StudentWorld*world, int startX, int startY, int iid, int hp);
	virtual void doSomething();

	bool grasshopperEatFood();

private:
	
};
class Adultgrasshopper : public Babygrasshopper
{
public:
	Adultgrasshopper(StudentWorld* world, int startX, int startY, int iid, int hp);
	virtual void doSomething();

private:
};// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
