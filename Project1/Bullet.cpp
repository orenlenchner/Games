#include "Bullet.h"
#include <GameEngine/ResourceManager.h>
#include "Human.h"
#include "Enemy.h"
#include "Level.h"

Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float d, float s, float r) :
	m_position(pos),
	m_direction(dir),
	m_damage(d),
	m_speed(s),
	m_range(r)
{
}


Bullet::~Bullet()
{
}

bool Bullet::update(const std::vector<std::string>& levelData, float deltaTime) {
	m_position += m_direction * m_speed * deltaTime;
	return collideWithWorld(levelData);
}

bool Bullet::rangeReached() {
	if (m_range <= 0) {
		return true;
	}
	else {
		m_range--;
		return false;
	}
}

void Bullet::draw(GameEngine::SpriteBatch& spriteBatch) {
	// Bullet size
	glm::vec4 destRect(m_position.x - BULLET_RADIUS, m_position.y - BULLET_RADIUS, BULLET_RADIUS * 2, BULLET_RADIUS *2);
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	// Bullet color and texture
	GameEngine::ColorRGBA8 color(32, 32, 32, 255);
	spriteBatch.draw(destRect, uvRect, GameEngine::ResourceManager::getTexture("Textures/circle.png").id, 0.0f, color);
}

bool Bullet::collideWithAgent(Agent* agent) {
	const float MIN_DISTANCE = AGENT_RADIUS + BULLET_RADIUS;

	// Center to center collision
	glm::vec2 centerPosA = m_position;
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS); 
	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);

	float collisionDepth = MIN_DISTANCE - distance;
	if (collisionDepth > 0) { return true; }
	return false;
}
bool Bullet::collideWithWorld(const std::vector<std::string>& levelData) {
	glm::vec2 gridPosition;
	gridPosition.x = floor(m_position.x / TILE_WIDTH);
	gridPosition.y = floor(m_position.y / TILE_WIDTH);

	if (gridPosition.x < 0 || gridPosition.x > levelData[0].size() ||
		gridPosition.y < 0 || gridPosition.y > levelData.size()) {
		return true;
	}
	return levelData[gridPosition.y][gridPosition.x] != '.';
}