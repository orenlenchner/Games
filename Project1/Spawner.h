#pragma once
#include <glm/glm.hpp>
#include <GameEngine/AudioEngine.h>
#include "Enemy.h"
#include <queue>

class Spawner
{
public:
	Spawner();
	~Spawner();

	// Places a spawner a a specific position and sets default values
	void init(glm::vec2 pos, std::vector<Enemy*>* m_enemies);

	// Keeps track of spawn timing
	void update();

	// Checks if the spawner has not spawned all the wave's enemies yet
	bool isActive();

	// Enqueue an enemy
	void enqueueEnemy(Enemy* newEnemy);

	// Spawn the enemy in the front of the queue
	Enemy* spawnNextEnemy();

	// Setters
	void setSpawnSpeed(float spawnSpeed);

	// Getters
	float getSpawnSpeed() { return m_spawnSpeed; }
	glm::vec2 getPosition() { return m_position; }

private:
	// Spawner values
	float m_spawnSpeed;
	glm::vec2 m_position;
	float m_spawnCounter;
	std::vector<Enemy*>* m_enemies;
	std::queue <Enemy*> m_spawnQueue;
};

