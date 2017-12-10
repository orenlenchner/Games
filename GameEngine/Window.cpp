#include "Window.h"
#include "EngineErrors.h"
#include <iostream>

namespace GameEngine {

	Window::Window()
	{
	}


	Window::~Window()
	{
	}

	int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags) {
		Uint32 flags = SDL_WINDOW_OPENGL;
		if (currentFlags & INVISIBLE) {
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (currentFlags & FULLSCREEN) {
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (currentFlags & BORDERLESS) {
			flags |= SDL_WINDOW_BORDERLESS;
		}
		// Open an SDL Window
		m_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);
		if (m_sdlWindow == nullptr) {
			fatalError("SDL Window could not be created!");
		}

		// Set up our OpenGl context
		SDL_GLContext glContext = SDL_GL_CreateContext(m_sdlWindow);
		if (glContext == nullptr) {
			fatalError("SDL_GL context could not be created!");
		}

		// Set up glew
		GLenum error = glewInit();
		if (error != GLEW_OK) {
			fatalError("Could not initialize glew!");
		}

		std::printf("***	OpenGL Version: %s	***\n", glGetString(GL_VERSION));

		// Sets the color of the background
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		// Set VSync (0 for immediate updates, 1 for updates synchronized with the vertical retrace, -1 for late swap tearing)
		SDL_GL_SetSwapInterval(-1);

		// Enable alpha channel blend
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}
	void Window::swapBuffer() {
		SDL_GL_SwapWindow(m_sdlWindow);
	}
}