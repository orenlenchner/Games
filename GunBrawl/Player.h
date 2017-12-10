#pragma once
#include "Human.h"
#include <GameEngine/InputManager.h>
#include <GameEngine/Camera2D.h>
#include "Gun.h"

class Player : public Human
{
public:
	Player();
	~Player();

	// Initializes the player
	void init(float speed, glm::vec2 pos, float health, GameEngine::InputManager* inputManager, GameEngine::Camera2D* camera, std::vector<Bullet>* bullets);

	// Gives the player a gun
	void addGun(Gun* gun);

	// Updates the player
	void update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Enemy*> enemies, float deltaTime) override;

	// Returns the players current gun
	Gun* getCurrentGun() { return m_guns[m_currentGunIndex]; }

	// Checks if the player swapped guns this fram
	bool swappedGuns() { return m_swappedThisFrame; }

private:
	GameEngine::InputManager* m_inputManager;

	std::vector<Gun*> m_guns;
	int m_currentGunIndex;
	bool m_swappedThisFrame = false;

	GameEngine::Camera2D* m_camera;
	std::vector<Bullet>* m_bullets;

	bool m_verticalPriority;
	bool m_horizontalPriority;
};

