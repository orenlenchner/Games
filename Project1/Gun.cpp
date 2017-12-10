#include "Gun.h"
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>

Gun::Gun(std::string name, std::vector<Bullet>* bullets, float fireRate, int bulletsPerShot, float spread, float bulletDamage,
	float bulletSpeed, float bulletRange, GameEngine::SoundEffect fireEffect) :
	m_name(name),
	m_bullets(bullets),
	m_fireRate(fireRate),
	m_bulletsPerShot(bulletsPerShot),
	m_spread(spread),
	m_bulletDamage(bulletDamage),
	m_bulletSpeed(bulletSpeed),
	m_bulletRange(bulletRange),
	m_frameCounter(0),
	m_fireEffect(fireEffect)
{
}

Gun::~Gun()
{
}

void Gun::update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, float deltaTime) {
	m_frameCounter += 1.0f + deltaTime;
	if (m_frameCounter >= m_fireRate && isMouseDown) {
		fire(position, direction, *m_bullets);
		m_frameCounter = 0;
	}
}

void Gun::fire(const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets) {
	
	// Calculate spread randomly
	static std::mt19937 randomEngine(time(nullptr));
	std::uniform_real_distribution<float> randRot(-m_spread, m_spread);

	// Play audio clip
	m_fireEffect.play();

	// Puts bullets into the active bullet list
	for (int i = 0; i < m_bulletsPerShot; i++) {
		bullets.emplace_back(position, glm::rotate(direction, randRot(randomEngine)), m_bulletDamage, m_bulletSpeed, m_bulletRange);
	}
}