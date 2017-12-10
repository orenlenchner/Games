#include "Human.h"
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>

Human::Human()
{
}


Human::~Human()
{
}

void Human::init(float speed, glm::vec2 pos) {
	m_health = 1000;
	m_speed = speed;
	m_position = pos;
}

void Human::update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Enemy*> enemies, float deltaTime) {
	collideWithLevel(levelData);
}
