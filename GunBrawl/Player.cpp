#include <SDL/SDL.h>
#include "Player.h"
#include "Gun.h"
#include <iostream>
#include <GameEngine/ResourceManager.h>

Player::Player() :
	m_currentGunIndex(-1)
{
}


Player::~Player()
{
	for (int i = 0; i < m_guns.size(); i++) {
		delete m_guns[i];
	}
}

void Player::init(float speed, glm::vec2 pos, float health, GameEngine::InputManager* inputManager, GameEngine::Camera2D* camera, std::vector<Bullet>* bullets) {
	m_health = health;
	m_speed = speed;
	m_position = pos;
	m_inputManager = inputManager;
	m_camera = camera;
	m_bullets = bullets;
	m_health = health;
	m_maxHealth = m_health;
	m_textureID = GameEngine::ResourceManager::getTexture("Textures/player.png").id;
}

void Player::addGun(Gun* gun) {
	m_guns.push_back(gun);

	// First time getting a gun
	if (m_currentGunIndex == -1) {
		m_currentGunIndex = 0;
	}
}

void Player::update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Enemy*> enemies, float deltaTime) {
	
	// Up, down, left, right movement for the player. Pressing a new key will change the direction to the new direction
	float x = 0;
	float y = 0;
	// W and S
	// Decides whether or not to override current vertical direction
	if (m_inputManager->isKeyPressed(SDLK_w)) {
		m_verticalPriority = true;
	}
	else if (m_inputManager->isKeyPressed(SDLK_s)) {
		m_verticalPriority = false;
	}
	if (m_inputManager->isKeyDown(SDLK_w) && m_inputManager->isKeyDown(SDLK_s)) {
		if (m_verticalPriority) {
			y += m_speed;
		}
		else if (!m_verticalPriority) {
			y -= m_speed;
		}
	}
	// Normal movement
	else if (m_inputManager->isKeyDown(SDLK_w)) {
		y += m_speed;
	}
	else if (m_inputManager->isKeyDown(SDLK_s)) {
		y -= m_speed;
	}

	// A and D
	// Decides whether or not to override current horizontal direction
	if (m_inputManager->isKeyPressed(SDLK_a)) {
		m_horizontalPriority = true;
	}
	else if (m_inputManager->isKeyPressed(SDLK_d)) {
		m_horizontalPriority = false;
	}
	if (m_inputManager->isKeyDown(SDLK_a) && m_inputManager->isKeyDown(SDLK_d)) {
		if (m_horizontalPriority) {
			x -= m_speed;
		}
		else if (!m_horizontalPriority) {
			x += m_speed;
		}
	}
	// Normal movement
	else if (m_inputManager->isKeyDown(SDLK_a)) {
		x -= m_speed;
	}
	else if (m_inputManager->isKeyDown(SDLK_d)) {
		x += m_speed;
	}
	
	// Diagonal normalization
	if (x != 0 && y != 0) {
		x /= sqrt(2);
		y /= sqrt(2);
	}

	// Push position changes
	m_position.x += x * deltaTime;
	m_position.y += y * deltaTime;

	// Reseting gun swap check;
	m_swappedThisFrame = false;

	// Using guns
	if (m_inputManager->isKeyDown(SDLK_1) && m_guns.size() >= 0) {
		m_currentGunIndex = 0;
		m_swappedThisFrame = true;
	}
	else if (m_inputManager->isKeyDown(SDLK_2) && m_guns.size() >= 1) {
		m_currentGunIndex = 1;
		m_swappedThisFrame = true;
	}
	else if (m_inputManager->isKeyDown(SDLK_3) && m_guns.size() >= 2) {
		m_currentGunIndex = 2;
		m_swappedThisFrame = true;
	}
	else if (m_inputManager->isKeyDown(SDLK_4) && m_guns.size() >= 3) {
		m_currentGunIndex = 3;
		m_swappedThisFrame = true;
	}
	glm::vec2 mouseCoords = m_inputManager->getMouseCoords();
	mouseCoords = m_camera->convertScreenToWorld(mouseCoords);
	glm::vec2 centerPosition = m_position + glm::vec2(AGENT_RADIUS, AGENT_RADIUS);
	m_direction = glm::normalize(mouseCoords - centerPosition);

	// Firing guns with mouse
	if (m_currentGunIndex != -1) {
		m_guns[m_currentGunIndex]->update(m_inputManager->isKeyDown(SDL_BUTTON_LEFT), centerPosition, m_direction, deltaTime);
	}
	
	collideWithLevel(levelData);
}