#pragma once

namespace GameEngine {
	class FPSLimiter {
	public:
		FPSLimiter();
		void init(float maxFPS);

		void setMaxFPS(float maxFPS);

		void begin();

		// end will return the current FPS
		int end();
	private:
		void calculateFPS();

		float m_fps;
		float m_maxFPS;
		float m_frameTime;

		unsigned int m_startTicks;
	};
}