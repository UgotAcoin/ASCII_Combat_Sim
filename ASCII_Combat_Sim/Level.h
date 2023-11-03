#pragma once
#include <vector>
#include<string>
#include "Soldier.h"

using namespace std;

const int NUM_ARMIES = 2;

class Level
{

public:
	Level();
	~Level();

	void load(string fileName);
	void print();

	void update();


	//getter
	char getTile(int x, int y);
	Soldier* getSoldier(int x, int y);
	//setter
	void setTile(int x, int y, char tile, Soldier *soldier);

private:
	void processSoldierMove(char direction, Soldier *soldier);
	void battle(Soldier *soldier, int targetX, int targetY);
	void moveSoldier(Soldier *soldier, int targetX, int targetY);


	vector <string> _levelData;
	vector <vector <Soldier *> > _soldierGird;
	vector <Soldier *> _armies[NUM_ARMIES];


};

