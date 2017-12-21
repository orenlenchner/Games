#include "MainGame.h"
#include <Windows.h>
#include <iostream>
int main(int argc, char** argv) {
	// Removes the console. Comment this line if game crashes
	//FreeConsole();

	MainGame mainGame;
	mainGame.run();

	return 0;
}