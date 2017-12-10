#pragma once

#include "Agent.h"
#include "Gun.h"
#include <float.h>

class Enemy : public Agent
{
public:
	Enemy();
	~Enemy();

	// Set all the enemies stats
	void init(float speed, glm::vec2 pos, float range, float damage, float health, float healthRegen, float score, Gun* gun, GLuint textureID, GameEngine::SoundEffect deathSound);

	// Update the enemies position; enemies move towards closest human (player)
	virtual void update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Enemy*> enemies, float deltaTime) override;

	// Return the enemies score
	int getScore() { return m_score; }
private:

	// Returns the closest human to this agent
	Human* getNearestHuman(std::vector<Human*>& humans);

	// Returns true if the enemy can "see" the closest human
	bool isInRange();

	// Enemy values
	float m_smallestDistance = FLT_MAX;
	float m_range;
	float m_score;
	Gun* m_gun;

};

