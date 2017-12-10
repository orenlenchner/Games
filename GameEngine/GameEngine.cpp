#include <SDL/SDL.h>
#include <GL/glew.h>
#include "GameEngine.h"

namespace GameEngine {

	int init() {
		// Initialize SDL
		SDL_Init(SDL_INIT_EVERYTHING);

		// Use SDL double buffered window (prevents flickering)
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return 0;
	}
}