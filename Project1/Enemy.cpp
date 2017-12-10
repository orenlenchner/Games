#include "Enemy.h"
#include "Human.h"
#include <iostream>
#include <GameEngine/ResourceManager.h>

int MIN_INT = -2147483648LL;

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	playDeathSound();
	delete m_gun;
}

void Enemy::init(float speed, glm::vec2 pos, float range, float damage, float health, float healthRegen, float score, Gun* gun, GLuint textureID, GameEngine::SoundEffect deathSound) {
	count = 0;
	m_speed = speed;
	m_position = pos;
	m_range = range;
	m_damage = damage;
	m_health = health;
	m_maxHealth = health;
	m_healthRegen = healthRegen;
	m_score = score;
	m_gun = gun;
	m_textureID = textureID;
	m_deathSound = deathSound;
}

void Enemy::update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Enemy*> enemies, float deltaTime) {
	
	// Get nearest human
	Human* closestHuman = getNearestHuman(humans);

	// Move towards nearest human
	if (closestHuman != nullptr) {
		m_direction = glm::normalize(closestHuman->getPosition() - m_position);
		glm::vec2 centerPosition = m_position + glm::vec2(AGENT_RADIUS, AGENT_RADIUS);
		m_position += m_direction * m_speed * deltaTime;
		if(m_gun != NULL) m_gun->update(isInRange(), centerPosition, m_direction, deltaTime);
	}

	// Update health
	regenHealth(m_healthRegen);

	// Check collision
	collideWithLevel(levelData);
}

Human* Enemy::getNearestHuman(std::vector<Human*>& humans) {
	Human* closestHuman = nullptr;
	float smallestDistance = FLT_MAX;

	for (int i = 0; i < humans.size(); i++) {
		glm::vec2 distVec = humans[i]->getPosition() - m_position;
		float distance = glm::length(distVec);

		if (distance < smallestDistance) {
			smallestDistance = distance;
			m_smallestDistance = smallestDistance;
			closestHuman = humans[i];
		}
	}

	return closestHuman;
}

bool Enemy::isInRange() {
	return m_smallestDistance < m_range;
}