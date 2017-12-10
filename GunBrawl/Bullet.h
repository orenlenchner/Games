#pragma once

#include <glm/glm.hpp>
#include <GameEngine/SpriteBatch.h>
#include <vector>

class Agent;
class Human;
class Enemy;

const int BULLET_RADIUS = 5;

class Bullet
{
public:
	// Initialize position, direction, damage, speed, and range
	Bullet(glm::vec2 pos, glm::vec2 dir, float d, float s, float r);
	~Bullet();

	// Update position over time
	bool update(const std::vector<std::string>& levelData, float deltaTime);

	// Draw to the world
	void draw(GameEngine::SpriteBatch& spriteBatch);

	// If the bullet has reached its maximum range
	bool rangeReached();

	// If the bullet collides with an agent
	bool collideWithAgent(Agent* agent);

	// Getters
	float getDamage() const { return m_damage; }
	glm::vec2 getPosition() const { return m_position; }

private:

	// Collision with world
	bool collideWithWorld(const std::vector<std::string>& levelData);

	// Bullet values
	glm::vec2 m_position;
	glm::vec2 m_direction;
	
	float m_damage;
	float m_speed;
	float m_range;
};

