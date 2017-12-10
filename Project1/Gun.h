#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Bullet.h"
#include <GameEngine/AudioEngine.h>

class Gun
{
public:
	Gun(std::string name, std::vector<Bullet>* bullets, float fireRate, int bulletsPerShot, float spread, float bulletDamage,
		float bulletSpeed, float bulletRange, GameEngine::SoundEffect fireEffect);
	~Gun();

	// Check if the gun should be fired or not
	void update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, float deltaTime);

	// Getters
	// Get the guns range
	float getRange() { return m_bulletRange; }
	// Get the name of the gun
	std::string getName() { return m_name; }

private:

	// Shoot the gun
	void fire(const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets);

	// Gun values
	GameEngine::SoundEffect m_fireEffect;
	std::string m_name;
	float m_fireRate;
	int m_bulletsPerShot;
	float m_spread;
	float m_bulletDamage;
	float m_bulletSpeed;
	float m_bulletRange;
	float m_frameCounter;
	std::vector<Bullet>* m_bullets;
};