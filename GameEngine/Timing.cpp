#include <SDL/SDL.h>
#include "Timing.h"

namespace GameEngine {
	FPSLimiter::FPSLimiter() {

	}
	void FPSLimiter::init(float maxFPS) {
		setMaxFPS(maxFPS);
	}

	void FPSLimiter::setMaxFPS(float maxFPS) {
		m_maxFPS = maxFPS;
	}


	void FPSLimiter::begin() {
		m_startTicks = SDL_GetTicks();
	}

	// end will return the current FPS
	int FPSLimiter::end() {
		calculateFPS();

		float frameTicks = (float)SDL_GetTicks() - m_startTicks;
		//Limit the FPS to the max FPS
		if (1000.0f / m_maxFPS > frameTicks) {
			SDL_Delay((int) (1000.0f / m_maxFPS - frameTicks));
		}

		return (int)m_fps;
	}

	void FPSLimiter::calculateFPS() {
		static const int NUM_SAMPLES = 10;
		static float frameTimes[NUM_SAMPLES];
		static int currentFrame = 0;

		static float prevTicks = (float)SDL_GetTicks();

		float currentTicks;
		currentTicks = (float)SDL_GetTicks();

		m_frameTime = currentTicks - prevTicks;
		frameTimes[currentFrame % NUM_SAMPLES] = m_frameTime;

		prevTicks = currentTicks;

		int count;

		currentFrame++;
		if (currentFrame < NUM_SAMPLES) {
			count = currentFrame;
		}
		else {
			count = NUM_SAMPLES;
		}

		float frameTimeAverage = 0;
		for (int i = 0; i < count; i++) {
			frameTimeAverage += frameTimes[i];
		}
		frameTimeAverage /= count;

		if (frameTimeAverage > 0) {
			m_fps = 1000.0f / frameTimeAverage;
		}
		else {
			m_fps = 0.0f;
		}
	}

}