#pragma once

#include <vector>

namespace GameEngine {

	class ParticleBatch2D;
	class SpriteBatch;

	class ParticleEngine2D
	{
	public:
		ParticleEngine2D();
		~ParticleEngine2D();

		// Adds a particle batch
		void addParticleBatch(ParticleBatch2D* particleBatch);

		// Updates all particle batches, deltaTime makes particles framerate independent
		void update(float deltaTime);

		// Draw the particles using a sprite batch
		void draw(SpriteBatch* spriteBatch);

	private:
		std::vector<ParticleBatch2D*> m_batches;
	};

}