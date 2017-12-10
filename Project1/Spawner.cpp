#include "Spawner.h"
#include <vector>
#include "Bullet.h"
#include <iostream>
#include "Gun.h"

const float DEFAULT_SPAWN_SPEED = 100.0f;
const float MIN_SPAWN_SPEED = 25;

Spawner::Spawner()
{
}


Spawner::~Spawner()
{
}

void Spawner::init(glm::vec2 pos, std::vector<Enemy*>* enemies) {
	m_position = pos;
	m_enemies = enemies;
	// Default spawner values
	m_spawnSpeed = DEFAULT_SPAWN_SPEED;
	m_spawnCounter = 0;
}

void Spawner::update() {
	// If the spawner is active spawn an enemies at the spawn speed
	if (isActive()) {
		if (m_spawnCounter++ == m_spawnSpeed) {
			m_spawnCounter = 0;
			m_enemies->push_back(spawnNextEnemy());
		}
	}
}

Enemy* Spawner::spawnNextEnemy() {
	Enemy* newEnemy = m_spawnQueue.front();
	m_spawnQueue.pop();
	return newEnemy;
}

bool Spawner::isActive() {
	return !m_spawnQueue.empty();
}

void Spawner::setSpawnSpeed(float spawnSpeed) {
	if (spawnSpeed < MIN_SPAWN_SPEED) {
		spawnSpeed = MIN_SPAWN_SPEED;
	}
	m_spawnSpeed = spawnSpeed;
}

void Spawner::enqueueEnemy(Enemy* newEnemy) {
	m_spawnQueue.push(newEnemy);
}