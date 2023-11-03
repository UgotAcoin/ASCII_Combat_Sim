#include <iostream>
#include <string>
#include <cstdio>
#include <conio.h>

#include "GameSystem.h"

using namespace std;

int main()
{
	GameSystem gamesystem("level1.txt");

	gamesystem.playGame();

	system("PAUSE");
	return 0;
}