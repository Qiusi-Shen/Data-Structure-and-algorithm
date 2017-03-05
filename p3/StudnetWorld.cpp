#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include "Actor.h"
#include "Field.h"
#include<vector>
#include "Compiler.h"

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Compiler;
class Anthill;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir), ticks(0), m_clean(false)
	{
	}
	virtual ~StudentWorld();

	virtual int init();
	

	virtual int move();
	

	virtual void cleanUp();

	bool canMove(int x, int y);
	void addActor(int row, int col, Actor* _actor);
	int returnID(int x, int y);
	std::vector<Actor*> getActorsAt(int row, int col) const;

	int prepareCompilers();
	Compiler* getCompilerForColony(int colonyNumber);

private:
	Field m_field;  //the grid
	std::vector<Actor*> m_actors[VIEW_HEIGHT][VIEW_WIDTH];//contains all actors
	bool loadTheField();
	void loadActors();
	void cleanDead(); //helper function called in move

	void updatePosition(Actor* q, int oldX, int oldY);  //helper in move function
	std::string updateDisplayText(); //called in move
	int ticks;
	bool m_clean;
	Compiler *compilerForEntrants[4]; // at most 4 entrants
	Anthill* m_anthills[4];
	int checkWinner();
};

#endif // STUDENTWORLD_H_
