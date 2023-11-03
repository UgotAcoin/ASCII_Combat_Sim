#include "GameSystem.h"
#include <conio.h>
#include <iostream>


GameSystem::GameSystem(string levelFileName)
{

	_level.load(levelFileName);

	system("PAUSE");

}

void GameSystem::playGame()
{

	bool isDone = false;

	while (isDone != true) {

		_level.print();

		_level.update();


		_getch();
	}

}
