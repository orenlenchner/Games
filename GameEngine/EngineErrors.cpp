#include "EngineErrors.h"
#include <cstdlib>
#include <iostream>
#include <SDL/SDL.h>

namespace GameEngine {

	// Prints out an error message and exits
	void fatalError(std::string errorString) {
		std::cout << errorString << std::endl;
		std::cout << "Enter any key to quit...";
		std::getchar();
		SDL_Quit();
		exit(1);
	}
}