#pragma once

#include <GameEngine/Window.h>
#include <GameEngine/GLSLProgram.h>
#include <GameEngine/Camera2D.h>
#include <GameEngine/InputManager.h>
#include "Level.h"
#include <GameEngine/Timing.h>
#include <GameEngine/SpriteBatch.h>
#include "Player.h"
#include "Enemy.h"
#include "Spawner.h"
#include "Bullet.h"
#include <GameEngine/SpriteFont.h>
#include <GameEngine/AudioEngine.h>
#include <GameEngine/ParticleEngine2D.h>
#include <GameEngine/ParticleBatch2D.h>

enum class GameState { PLAY, EXIT, MENU, GAMEOVER };

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:
	// Initializes the core systems
	void initSystems();

	// Initializes the level and sets up everything
	void initLevel();

	// Initializes the shaders
	void initShaders();

	// Main game loop for the program
	void gameLoop();

	// Updates the spawners
	void updateSpawners();

	// Updates all agents
	void updateAgents(float deltaTime);

	// Updates all bullets
	void updateBullets(float deltaTime);

	// Check the victory condition
	void checkVictory();
	
	// Begin next wave of enemies
	void startNextWave();

	// Check if all spawners are inactive
	bool allSpawnersInactive();

	// Handle input processing
	void processInput();
	
	// Renders the game
	void drawGame();

	// Simple manager for the pause menu
	void menuManager();

	// Draws the HUD
	void drawHUD();

	// Checks if camera is locked
	void updateCamera();

	// Addes blood to the particle engine
	void addBlood(const glm::vec2& position, int numParticles);

	/***** Member Variables *****/
	GameEngine::Window m_window; // The game window

	GameEngine::Window m_fullscreenWindow;

	GameEngine::GLSLProgram m_textureProgram; // The shader program

	GameEngine::InputManager m_inputManager; // Handles input

	// Camera elements
	GameEngine::Camera2D m_camera; // Main camera
	GameEngine::Camera2D m_staticCamera; // Static camera
	bool m_lockedCam = true;

	// Sprite elements
	GameEngine::SpriteBatch m_agentSpriteBatch;
	GameEngine::SpriteBatch m_hudSpriteBatch;

	// Particle elements
	GameEngine::ParticleEngine2D m_particleEngine;
	GameEngine::ParticleBatch2D* m_bloodParticleBatch;

	std::vector<Level*> m_levels;

	int m_screenWidth;
	int m_screenHeight;
	float m_screenScaleMax;
	float m_screenScaleMin;
	GameState m_gameState;
	int m_currentLevel;
	bool m_victory = false;
	int m_score;

	bool m_godMode = false;

	float m_maxFPS;
	int m_fps;
	GameEngine::FPSLimiter m_fpsLimiter;

	Player* m_player;
	std::vector<Human*> m_humans;
	std::vector<Enemy*> m_enemies;
	std::vector<Spawner*> m_spawners;
	std::vector<Bullet> m_bullets;
	std::vector<Bullet> m_enemyBullets;

	int m_numHumansKilled;
	int m_numEnemysKilled;

	int m_waves;
	int m_maxWaves;
	
	// HUD elements
	int m_gunSelectFade;
	bool m_instructionMenu = false;
	bool m_fullScreen = false;
	GameEngine::SpriteFont* m_spriteFont;

	// Audio
	GameEngine::AudioEngine m_audioEngine;
	bool m_musicOn = false;

	std::string m_version = "1.0";
};