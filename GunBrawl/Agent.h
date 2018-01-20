#pragma once

#include <glm/glm.hpp>
#include <GameEngine/SpriteBatch.h>
#include <GameEngine/AudioEngine.h>

const float AGENT_WIDTH = 64.0f;
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;
const float HEALTH_REGEN_RATE = 10;

class Enemy;
class Human;

class Agent
{
public:
	Agent();
	virtual ~Agent();

	// Update the agents position
	virtual void update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Enemy*> enemies, float deltaTime) = 0;

	// Collision with the world
	bool collideWithLevel(const std::vector<std::string>& levelData);

	// Collision with other agentts
	bool collideWithAgent(Agent* agent);

	// Draw to the world
	void draw(GameEngine::SpriteBatch& m_spriteBatch);

	// Return true if agent is dead
	bool applyDamage(float damage);

	// Death sound
	void playDeathSound() { m_deathSound.play(); }

	// Setters
	void setHealth(float health) { m_health = health; }

	// Getters
	glm::vec2 getPosition() const { return m_position; }
	int getHealth() const { if ((int)m_health < 0) { return 0; } else { return (int)m_health; } }
	int getMaxHealth() const { return (int)m_maxHealth; }
	float getDamage() { return m_damage; }


protected:

	// Collision functions
	void checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePositions, float x, float y);
	void collideWithTile(glm::vec2 tilePos);

	// If the agent has health regen
	void regenHealth(float healthRegen);

	// Agent values
	glm::vec2 m_position;
	glm::vec2 m_direction = glm::vec2(1.0f, 0.0f);
	GameEngine::ColorRGBA8 m_color = GameEngine::ColorRGBA8(255, 255, 255, 255);
	float m_speed;
	float m_damage = 1; // default "touch" damage for all agents
	float m_health;
	float m_maxHealth;
	float m_healthRegen;
	GameEngine::SoundEffect m_deathSound;
	int count;

	GLuint m_textureID;
};

