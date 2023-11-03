#include "Level.h"
#include <fstream>
#include <iostream>
#include <cstdio>
#include <random>

using namespace std;

Level::Level()
{

}

Level::~Level()
{
	for (int i = 0; i < NUM_ARMIES; i++) {
		for (int j = 0; _armies[i].size(); j++) {
			delete _armies[i][j];
		}
	}
}

void Level::load(string fileName)
{

	//loads the level
	ifstream file;

	file.open(fileName);
	if (file.fail()) {
		perror(fileName.c_str());
		system("PAUSE");
		exit(1);
	}

	string line;

	while (getline(file, line)) {
		_levelData.push_back(line);
		_soldierGird.push_back(vector <Soldier *>());
		_soldierGird.back().resize(line.size(), nullptr);
	}

	file.close();

	//process the level

	char tile;
	for (int i = 0; i < _levelData.size(); i++) {
		for (int j = 0; j < _levelData[i].size(); j++) {
			tile = _levelData[i][j];

			switch (tile) {

			case '1': //team 1
				_armies[0].push_back(new Soldier("1", tile, 1, 10, 5, 10, 0));
				_armies[0].back()->setPosition(j, i);
				_soldierGird[i][j] = _armies[0].back();
				break;
			case '2': //team 2
				_armies[1].push_back(new Soldier("2", tile, 1, 10, 5, 10, 1));
				_armies[1].back()->setPosition(j, i);
				_soldierGird[i][j] = _armies[1].back();
			case '#':
			case '*':
				break;
			default:
				printf("WARNING : Unknown tile %c at %d,%d", tile, j, i);
				system("PAUSE");
				break;
			}
		}
	}
	random_device::result_type seed = std::random_device()();
	mt19937 randomEngine(seed);
	Soldier* tmp;


	//shuffle armies
	for (int i = 0; i < NUM_ARMIES; i++) {
		for (int j = _armies[i].size() - 1; j > 1; j--) {
			uniform_int_distribution<int> randomSwap(0, j - 1);
			int swap = randomSwap(randomEngine);
			tmp = _armies[i][j];
			_armies[i][j] = _armies[i][swap];
			_armies[i][swap] = tmp;
		}
	}
}

void Level::print()
{
	cout << string(100, '\n');

	for (int i = 0; i < _levelData.size(); i++) {
		printf("%s\n", _levelData[i].c_str());
	}
	printf("\n");
}

void Level::update()
{
	char move;

	int i = 0;

	bool isDone = false;

	while (isDone == false) {
		isDone = true;
		for (int j = 0; j < NUM_ARMIES; j++) {
			if (i < _armies[j].size()) {
				move = _armies[j][i]->getMove(_armies, NUM_ARMIES);
				processSoldierMove(move, _armies[j][i]);
				isDone = false;
			}
		}
		i++;
	}
}

char Level::getTile(int x, int y)
{
	return _levelData[y][x];
}

void Level::setTile(int x, int y, char tile, Soldier *soldier)
{
	_levelData[y][x] = tile;
	_soldierGird[y][x] = soldier;
}

Soldier* Level::getSoldier(int x, int y)
{
	return _soldierGird[y][x];
}

void Level::processSoldierMove(char direction, Soldier *soldier)
{
	int x, y;
	int targetX, targetY;

	soldier->getPosition(x, y);

	switch (direction) {
	case 'w'://up
		targetX = x;
		targetY = y - 1;
		break;
	case 'a'://left
		targetX = x - 1;
		targetY = y;
		break;
	case 's'://down
		targetX = x;
		targetY = y + 1;
		break;
	case 'd'://right
		targetX = x + 1;
		targetY = y;
		break;
	case '*':
		break;
	}

	char targetTile = getTile(targetX, targetY);

	switch (targetTile)
	{
	case '#':
		break;
	case '*':
		moveSoldier(soldier, targetX, targetY);
		break;
	default:
		battle(soldier, targetX, targetY);
		break;
	}

}

void Level::battle(Soldier *soldier, int targetX, int targetY)
{
	int x, y;
	int enemyArmy;
	soldier->getPosition(x, y);

	Soldier *targetSoldier = getSoldier(targetX, targetY);
	if (targetSoldier == nullptr) {
		return;
	}
	enemyArmy = targetSoldier->getArmy();
	if (enemyArmy == soldier->getArmy()) {
		return;
	}

	int result = targetSoldier->takeDamage(soldier->attack());
	if (result == 1) {
		for (int i = 0; i < _armies[enemyArmy].size(); i++) {
			if (_armies[enemyArmy][i] == targetSoldier) {
				_armies[enemyArmy][i] = _armies[enemyArmy].back();
				_armies[enemyArmy].pop_back();

				delete targetSoldier;

				setTile(targetX, targetY, '*', nullptr);
				break;
			}
		}
	}

}

void Level::moveSoldier(Soldier *soldier, int targetX, int targetY)
{
	int x, y;
	soldier->getPosition(x, y);

	setTile(x, y, '*', nullptr);
	setTile(targetX, targetY, soldier->getTile(), soldier);

	soldier->setPosition(targetX, targetY);
}