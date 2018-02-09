#define GLEW_STATIC

#include "MainGame.h"

#include <GameEngine/GameEngine.h>
#include <GameEngine/ResourceManager.h>
#include <iostream>
#include "Enemy.h"
#include <random>
#include <ctime>
#include <GameEngine/EngineErrors.h>
#include "Gun.h"
#include <algorithm>
#include <glm/gtx/rotate_vector.hpp>

/***** CONSTANTS ******/

// Player
const float PLAYER_MAX_HEALTH = 2000.0f;
const float PLAYER_SPEED = 15.0f;
const float HUMAN_SPEED = 1.0f;

// Spawners
const float DEFAULT_SPAWN_SPEED = 100.0f;
const float SPAWN_SPEED_MULTIPLIER = 3.0f;

// Player Guns
// Pistol
const char* PLAYER_PISTOL_NAME = "Pistol";
const float PLAYER_PISTOL_FIRERATE = 50;
const int PLAYER_PISTOL_BULLETS_PER_SHOT = 1;
const float PLAYER_PISTOL_SPREAD = 0.0f;
const float PLAYER_PISTOL_DAMAGE = 1000.0f;
const float PLAYER_PISTOL_BULLET_SPEED = 40.0f;
const float PLAYER_PISTOL_RANGE = 300.0f;

// Shotgun
const char* PLAYER_SHOTGUN_NAME = "Shotgun";
const float PLAYER_SHOTGUN_FIRERATE = 100;
const int PLAYER_SHOTGUN_BULLETS_PER_SHOT = 10;
const float PLAYER_SHOTGUN_SPREAD = 0.1f;
const float PLAYER_SHOTGUN_DAMAGE = 150.0f;
const float PLAYER_SHOTGUN_BULLET_SPEED = 30.0f;
const float PLAYER_SHOTGUN_RANGE = 45.0f;

// Minigun
const char* PLAYER_MINIGUN_NAME = "Minigun";
const float PLAYER_MINIGUN_FIRERATE = 12;
const int PLAYER_MINIGUN_BULLETS_PER_SHOT = 1;
const float PLAYER_MINIGUN_SPREAD = 0.1f;
const float PLAYER_MINIGUN_DAMAGE = 200.0f;
const float PLAYER_MINIGUN_BULLET_SPEED = 20.0f;
const float PLAYER_MINIGUN_RANGE = 75.0f;

// Sniper
const char* PLAYER_SNIPER_NAME = "Sniper";
const float PLAYER_SNIPER_FIRERATE = 130;
const int PLAYER_SNIPER_BULLETS_PER_SHOT = 10;
const float PLAYER_SNIPER_SPREAD = 0.0f;
const float PLAYER_SNIPER_DAMAGE = 250.0f;
const float PLAYER_SNIPER_BULLET_SPEED = 50.0f;
const float PLAYER_SNIPER_RANGE = 1000.0f;

// Enemies
// Pistol Enemy
const float PISTOL_ENEMY_SPEED = 4.0f;
const float PISTOL_ENEMY_RANGE = 1000.0f;
const float PISTOL_ENEMY_DAMAGE = 1.0f;
const float PISTOL_ENEMY_HEALTH = 200.0f;
const float PISTOL_ENEMY_HEALTH_REGEN = 0.0f;
const float PISTOL_ENEMY_SCORE = 10.0f;

const float PISTOL_ENEMY_LASTSPAWNWAVE = 10;

// Shotgun Enemy
const float SHOTGUN_ENEMY_SPEED = 6.0f;
const float SHOTGUN_ENEMY_RANGE = 700.0f;
const float SHOTGUN_ENEMY_DAMAGE = 1.0f;
const float SHOTGUN_ENEMY_HEALTH = 500.0f;
const float SHOTGUN_ENEMY_HEALTH_REGEN = 0.0f;
const float SHOTGUN_ENEMY_SCORE = 20.0f;

const float SHOTGUN_ENEMY_STARTSPAWNWAVE = 5;

// Will spawn numWaves - SHOTGUN_ENEMY_SPAWNNUMBEROFFSET enemies each wave
const float SHOTGUN_ENEMY_SPAWNNUMBEROFFSET = 5;

// Bomb Enemy
const float BOMB_ENEMY_SPEED = 9.0f;
const float BOMB_ENEMY_RANGE = 1000.0f;
const float BOMB_ENEMY_DAMAGE = 250.0f;
const float BOMB_ENEMY_HEALTH = 0.0f;
const float BOMB_ENEMY_HEALTH_REGEN = 0.0f;
const float BOMB_ENEMY_SCORE = 10.0f;

const float BOMB_ENEMY_STARTSPAWNWAVE = 10;

// Will spawn numWaves - BOMB_ENEMY_SPAWNNUMBEROFFSET enemies each wave
const float BOMB_ENEMY_SPAWNNUMBEROFFSET = 5;

// Enemy guns
// Pistol
const char* PISTOL_NAME = "EnemyPistol";
const float PISTOL_FIRERATE = 120;
const int PISTOL_BULLETS_PER_SHOT = 1;
const float PISTOL_SPREAD = 0.0f;
const float PISTOL_DAMAGE = 100.0f;
const float PISTOL_BULLET_SPEED = 30.0f;
const float PISTOL_RANGE = 80.0f;

// Pistol
const char* SHOTGUN_NAME = "EnemyShotgun";
const float SHOTGUN_FIRERATE = 170;
const int SHOTGUN_BULLETS_PER_SHOT = 5;
const float SHOTGUN_SPREAD = 0.1f;
const float SHOTGUN_DAMAGE = 20.0f;
const float SHOTGUN_BULLET_SPEED = 30.0f;
const float SHOTGUN_RANGE = 120.0f;

// Camera and Font
const float CAMERA_SCALE_CONSTANT = 2440.0f;
const float CAMERA_SPEED = 2.0f;
const float CAMERA_SCALE_SPEED = 0.1f;
const int CAMERA_BORDER_SIZE = 64;
const float CAMERA_MOV_SPEED = 20.0f;
const int FONT_RESOLUTION = 32;

// Time step
const int MAX_PHYSICS_STEPS = 6;
const float DESIRED_FPS = 60.0f;
const float MS_PER_SECOND = 1000.0f;
const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;
const float MAX_DELTA_TIME = 1.0f;

// Particles
const int BLOOD_PARTICLE_COUNT = 8;
const int MAX_BLOOD_PARTICLES = 1000;
const float BLOOD_DECAY_RATE = 0.05f;

// Score
const float SCORE_MULTIPLIER_PER_WAVE = 10;

/********************/

MainGame::MainGame() :
	m_screenWidth(1280),
	m_screenHeight(720),
	m_gameState(GameState::MENU),
	m_maxFPS(DESIRED_FPS),
	m_player(nullptr),
	m_numHumansKilled(0),
	m_numEnemysKilled(0),
	m_waves(0),
	m_maxWaves(15),
	m_gunSelectFade(255),
	m_score(0)
{
}


MainGame::~MainGame() {
	for (unsigned int i = 0; i < m_levels.size(); i++) {
		delete m_levels[i];
	}
	for (unsigned int i = 0; i < m_humans.size(); i++) {
		delete m_humans[i];
	}
	for (unsigned int i = 0; i < m_enemies.size(); i++) {
		delete m_enemies[i];
	}
}


void MainGame::run() {
	initSystems();
	initLevel();
	gameLoop();
}


void MainGame::initSystems() {
	// Initialize the game engine
	GameEngine::init();

	// Initialize game audio
	m_audioEngine.init();

	// Create our window
	m_window.create("Gun Brawl " + m_version, m_screenWidth, m_screenHeight, 0);
	// Black background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Set up the shaders
	initShaders();

	// Initialize the sprite batches
	m_agentSpriteBatch.init();
	m_hudSpriteBatch.init();

	// Initialize sprite font
	m_spriteFont = new GameEngine::SpriteFont("Fonts/Pixelate.ttf", FONT_RESOLUTION);


	// Set up the camera
	m_camera.init(m_screenWidth, m_screenHeight);
	m_screenScaleMax = m_screenWidth / CAMERA_SCALE_CONSTANT;
	m_screenScaleMin = m_screenScaleMax * 2;
	m_staticCamera.init(m_screenWidth, m_screenHeight);
	m_staticCamera.setPosition(glm::vec2(m_screenWidth / 2, m_screenHeight / 2));

	// Initialize particles
	m_bloodParticleBatch = new GameEngine::ParticleBatch2D;
	m_bloodParticleBatch->init(MAX_BLOOD_PARTICLES, BLOOD_DECAY_RATE, GameEngine::ResourceManager::getTexture("Textures/circle.png"),
		// Lambda
		[](GameEngine::Particle2D& particle, float deltaTime) {
		particle.position += particle.velocity * deltaTime;
		particle.color.a = (GLubyte(particle.life * 255.0f));
	});
	m_particleEngine.addParticleBatch(m_bloodParticleBatch);
}

void MainGame::initLevel() {
	m_levels.push_back(new Level("Levels/Level0.txt"));
	m_currentLevel = 0;

	m_player = new Player();
	m_player->init(PLAYER_SPEED, m_levels[m_currentLevel]->getStartPlayerPos(), PLAYER_MAX_HEALTH, &m_inputManager, &m_camera, &m_bullets);

	m_humans.push_back(m_player);

	// Init Pistol Enemy


	// Init Spawners
	const std::vector<glm::vec2>& spawnerPositions = m_levels[m_currentLevel]->getEnemySpawnPos();
	for (int i = 0; i < spawnerPositions.size(); i++) {
		// Create a spawner at every spawner position
		m_spawners.push_back(new Spawner);
		m_spawners.back()->init(spawnerPositions[i], &m_enemies);
	}
	
	// Start the first wave
	startNextWave();

	// Player Guns
	m_player->addGun(new Gun(PLAYER_PISTOL_NAME, &m_bullets, PLAYER_PISTOL_FIRERATE, PLAYER_PISTOL_BULLETS_PER_SHOT, PLAYER_PISTOL_SPREAD,
		PLAYER_PISTOL_DAMAGE, PLAYER_PISTOL_BULLET_SPEED, PLAYER_PISTOL_RANGE, m_audioEngine.loadSoundEffect("Sounds/pistol.wav")));
	m_player->addGun(new Gun(PLAYER_SHOTGUN_NAME, &m_bullets, PLAYER_SHOTGUN_FIRERATE, PLAYER_SHOTGUN_BULLETS_PER_SHOT, PLAYER_SHOTGUN_SPREAD,
		PLAYER_SHOTGUN_DAMAGE, PLAYER_SHOTGUN_BULLET_SPEED, PLAYER_SHOTGUN_RANGE, m_audioEngine.loadSoundEffect("Sounds/shotgun.wav")));
	m_player->addGun(new Gun(PLAYER_MINIGUN_NAME, &m_bullets, PLAYER_MINIGUN_FIRERATE, PLAYER_MINIGUN_BULLETS_PER_SHOT, PLAYER_MINIGUN_SPREAD,
		PLAYER_MINIGUN_DAMAGE, PLAYER_MINIGUN_BULLET_SPEED, PLAYER_MINIGUN_RANGE, m_audioEngine.loadSoundEffect("Sounds/minigun.wav")));
	m_player->addGun(new Gun(PLAYER_SNIPER_NAME, &m_bullets, PLAYER_SNIPER_FIRERATE, PLAYER_SNIPER_BULLETS_PER_SHOT, PLAYER_SNIPER_SPREAD,
		PLAYER_SNIPER_DAMAGE, PLAYER_SNIPER_BULLET_SPEED, PLAYER_SNIPER_RANGE, m_audioEngine.loadSoundEffect("Sounds/sniper.wav")));
}

void MainGame::initShaders() {
	m_textureProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();
}

void MainGame::gameLoop() {
	m_camera.setScale(m_screenScaleMax);
	m_fpsLimiter.setMaxFPS(m_maxFPS);

	float prevTicks = SDL_GetTicks();

	// Main loop
	while (m_gameState != GameState::EXIT) {

		m_fpsLimiter.begin();

		checkVictory();
		
		m_inputManager.update();
		processInput();

		if (m_gameState == GameState::PLAY) {
			if (!m_musicOn) {
				GameEngine::Music music = m_audioEngine.loadMusic("Sounds/Killers.wav");
				music.play(-1);
				m_musicOn = true;
			}

			// Time Step (Semi-fixed)
			float newTicks = SDL_GetTicks();
			float frameTime = newTicks - prevTicks;
			prevTicks = newTicks;
			float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

			// Update based on time step
			int i = 0;
			while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) {
				float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
				updateSpawners();
				updateAgents(deltaTime);
				updateBullets(deltaTime);
				// Particles
				m_particleEngine.update(deltaTime);

				totalDeltaTime -= deltaTime;
				i++;
			}
		}
		// Camera
		updateCamera();
		m_camera.update();
		m_staticCamera.update();

		// Draw all elements to the screen
		drawGame();

		// Manage user input to pause/quit
		menuManager();

		m_fps = m_fpsLimiter.end();
	}
}

void MainGame::menuManager() {
	// Gameover
	if (m_inputManager.isKeyPressed(SDLK_ESCAPE)) {
		m_gameState = GameState::EXIT;
	}
	else if (m_gameState == GameState::PLAY) {
		if (m_inputManager.isKeyPressed(SDLK_SPACE)) {
			m_instructionMenu = false;
			m_gameState = GameState::MENU;
		}
	}
	else if (m_gameState == GameState::MENU){
		// Main menu
		if (m_inputManager.isKeyPressed(SDLK_i)) {
			m_instructionMenu = !m_instructionMenu;
		}
		if (m_inputManager.isKeyPressed(SDLK_SPACE)) {
			if (m_gameState == GameState::MENU) {
				m_gameState = GameState::PLAY;
			}
		}
		// Fullscreen mode
		if (m_inputManager.isKeyPressed(SDLK_f)) {
			m_fullScreen = !m_fullScreen;
			if (m_fullScreen) {
				SDL_SetWindowFullscreen(m_window.getWindow(), SDL_TRUE);
			}
			else {
				SDL_SetWindowFullscreen(m_window.getWindow(), SDL_FALSE);
			}
		}
		// God mode
		if (m_inputManager.isKeyPressed(SDLK_g)) {
			m_godMode = !m_godMode;
		}
	}
}

void MainGame::updateSpawners() {
	// Update Spawners
	for (int i = 0; i < m_spawners.size(); i++) {
		m_spawners[i]->update();
	}
}

void MainGame::updateAgents(float deltaTime) {

	// Update player
	m_player->update(m_levels[m_currentLevel]->getLevelData(), m_humans, m_enemies, deltaTime);

	// Update all humans
	for (unsigned int i = 1; i < m_humans.size(); i++) {
		m_humans[i]->update(m_levels[m_currentLevel]->getLevelData(), m_humans, m_enemies, deltaTime);
	}

	// Update all enemies
	for (unsigned int i = 0; i < m_enemies.size(); i++) {
		m_enemies[i]->update(m_levels[m_currentLevel]->getLevelData(), m_humans, m_enemies, deltaTime);
	}

	// Human-human collision
	for (int i = 0; i < m_humans.size(); i++) {
		for (int j = i + 1; j < m_humans.size(); j++) {
			m_humans[i]->collideWithAgent(m_humans[j]);
		}
	}
	// Enemy collision
	for (int i = 0; i < m_enemies.size(); i++) {
		// Enemy-player
		if (m_enemies[i]->collideWithAgent(m_player)) {
			m_score -= m_enemies[i]->getDamage();
			// Damage human
			if (!m_godMode) {
				if (m_player->applyDamage(m_enemies[i]->getDamage())) {
					// If the player end the game
					m_gameState = GameState::GAMEOVER;
				}
			}
			if (m_enemies[i]->applyDamage(m_player->getDamage())) {
				// If the enemy died remove it
				//m_enemies[i]->playDeathSound();
				delete m_enemies[i];
				m_enemies[i] = m_enemies.back();
				m_enemies.pop_back();
				m_numEnemysKilled++;
			}
		}
		// Enemy-enemy
		for (int j = i + 1; j < m_enemies.size(); j++) {
			m_enemies[i]->collideWithAgent(m_enemies[j]);
		}
		// Enemy-human
		for (int j = 1; j < m_humans.size(); j++) {
			if (m_enemies[i]->collideWithAgent(m_humans[j])) {
				// Damage human
				if (m_humans[j]->applyDamage(m_enemies[i]->getDamage())) {
					// If the human died remove it
					delete m_humans[j];
					m_humans[j] = m_humans.back();
					m_humans.pop_back();
					m_numHumansKilled++;
				}
			}
		}
	}
}

void MainGame::updateBullets(float deltaTime) {
	/**** Player Bullets ****/
	// Update and collide with world
	for (int i = 0; i < m_bullets.size(); ) {
		// If update return true, the bullet  collided with a wall
		if (m_bullets[i].update(m_levels[m_currentLevel]->getLevelData(), deltaTime) || m_bullets[i].rangeReached()) {
			m_bullets[i] = m_bullets.back();
			m_bullets.pop_back();
		}
		else {
			i++;
		}

		bool wasBulletRemoved;

		// Collide with humans and enemies
		for (int i = 0; i < m_bullets.size(); i++) {
			wasBulletRemoved = false;
			// Loop through enemies
			for (int j = 0; j < m_enemies.size(); ) {
				// Check collision
				if (m_bullets[i].collideWithAgent(m_enemies[j])) {
					// Add blood
					addBlood(m_bullets[i].getPosition(), BLOOD_PARTICLE_COUNT);
					// Damage enemy
					if (m_enemies[j]->applyDamage(m_bullets[i].getDamage())) {
						// If the enemy died remove it
						m_score += m_enemies[j]->getScore() + SCORE_MULTIPLIER_PER_WAVE * m_waves;
						delete m_enemies[j];
						m_enemies[j] = m_enemies.back();
						m_enemies.pop_back();
						m_numEnemysKilled++;
					}
					else {
						j++;
					}
					// Remove the bullet
					m_bullets[i] = m_bullets.back();
					m_bullets.pop_back();

					wasBulletRemoved = true;
					// Make sure we don't skip a bullet
					i--;
					// Since the bullet died, no need to loop through more enemies
					break;
				}
				else {
					j++;
				}
			}
			// Loop through humans
			if (wasBulletRemoved == false) {
				for (int j = 1; j < m_humans.size(); ) {
					// Check collision
					if (m_bullets[i].collideWithAgent(m_humans[j])) {
						// Add blood
						addBlood(m_bullets[i].getPosition(), BLOOD_PARTICLE_COUNT);
						// Damage human
						if (m_humans[j]->applyDamage(m_bullets[i].getDamage())) {
							// If the human died remove it
							delete m_humans[j];
							m_humans[j] = m_humans.back();
							m_humans.pop_back();
							m_numHumansKilled++;
						}
						else {
							j++;
						}
						// Remove the bullet
						m_bullets[i] = m_bullets.back();
						m_bullets.pop_back();
						// Make sure we don't skip a bullet
						i--;
						// Since the bullet died, no need to loop through more enemies
						break;
					}
					else {
						j++;
					}
				}
			}
		}
	}
	/**** Enemy Bullets ****/
	// Update and collide with world
	for (int i = 0; i < m_enemyBullets.size(); ) {
		// If update return true, the bullet collided with a wall
		if (m_enemyBullets[i].update(m_levels[m_currentLevel]->getLevelData(), deltaTime) || m_enemyBullets[i].rangeReached()) {
			m_enemyBullets[i] = m_enemyBullets.back();
			m_enemyBullets.pop_back();
		}
		else {
			i++;
		}

		// Collide with player and humans
		for (int i = 0; i < m_enemyBullets.size(); i++) {
			// Check player
			if (m_enemyBullets[i].collideWithAgent(m_player)) {
				addBlood(m_enemyBullets[i].getPosition(), BLOOD_PARTICLE_COUNT);
				m_score -= m_enemyBullets[i].getDamage();
				if (!m_godMode) {
					if (m_player->applyDamage(m_enemyBullets[i].getDamage())) {
						// If the player died end the game it
						m_gameState = GameState::GAMEOVER;
					}
				}
				// Remove the bullet
				m_enemyBullets[i] = m_enemyBullets.back();
				m_enemyBullets.pop_back();
				// Make sure we don't skip a bullet
				i--;
				// Since the bullet died, no need to loop through humans
				break;
			}
			// Loop through humans
			for (int j = 1; j < m_humans.size(); ) {
				// Check collision
				if (m_enemyBullets[i].collideWithAgent(m_humans[j])) {
					// Add blood
					addBlood(m_enemyBullets[i].getPosition(), BLOOD_PARTICLE_COUNT);
					// Damage human
					if (m_humans[j]->applyDamage(m_enemyBullets[i].getDamage())) {
						// If the human died remove it
						delete m_humans[j];
						m_humans[j] = m_humans.back();
						m_humans.pop_back();
						m_numHumansKilled++;
					}
					else {
						j++;
					}
					// Remove the bullet
					m_enemyBullets[i] = m_enemyBullets.back();
					m_enemyBullets.pop_back();
					// Make sure we don't skip a bullet
					i--;
					// Since the bullet died, no need to loop through more humans
					break;
				}
				else {
					j++;
				}
			}
		}
	}
}

void MainGame::checkVictory() {
	// Checks if all enemies are dead and all spawners have finished spawning
	if (m_enemies.empty() && allSpawnersInactive()) {
		if (m_waves >= m_maxWaves) {
			m_victory = true;
			m_gameState = GameState::GAMEOVER;
		}
		else {
			startNextWave();
		}
	}
}

bool MainGame::allSpawnersInactive() {
	// Checks spawners are active
	for (int i = 0; i < m_spawners.size(); i++) {
		if (m_spawners[i]->isActive()) {
			return false;
		}
	}
	return true;
}

void MainGame::startNextWave() {
	m_waves++;
	m_player->setHealth(m_player->getMaxHealth());
	// Set enemy spawn numbers for the wave
	for (int i = 0; i < m_spawners.size(); i++) {
		// Increase spawn speed
		m_spawners[i]->setSpawnSpeed(DEFAULT_SPAWN_SPEED - (SPAWN_SPEED_MULTIPLIER * m_waves));
		// Pistol Enemies
		if (m_waves <= PISTOL_ENEMY_LASTSPAWNWAVE) {
			for (int j = 0; j < m_waves; j++) {
				Enemy* pistolEnemy = new Enemy;
				Gun* pistolEnemyGun = new Gun(PISTOL_NAME, &m_enemyBullets, PISTOL_FIRERATE, PISTOL_BULLETS_PER_SHOT, PISTOL_SPREAD,
					PISTOL_DAMAGE, PISTOL_BULLET_SPEED, PISTOL_RANGE, m_audioEngine.loadSoundEffect("Sounds/pistol.wav"));
				pistolEnemy->init(PISTOL_ENEMY_SPEED, m_spawners[i]->getPosition(), PISTOL_ENEMY_RANGE, PISTOL_ENEMY_DAMAGE,
					PISTOL_ENEMY_HEALTH, PISTOL_ENEMY_HEALTH_REGEN, PISTOL_ENEMY_SCORE, pistolEnemyGun,
					GameEngine::ResourceManager::getTexture("Textures/enemyGreen.png").id, m_audioEngine.loadSoundEffect("Sounds/pop.wav"));
				m_spawners[i]->enqueueEnemy(pistolEnemy);
			}
		}
		// Shotgun Enemies
		if (m_waves > SHOTGUN_ENEMY_STARTSPAWNWAVE) {
			for (int j = SHOTGUN_ENEMY_SPAWNNUMBEROFFSET; j < m_waves; j++) {
				Enemy* shotgunEnemy = new Enemy;
				Gun* shotgunEnemyGun = new Gun(SHOTGUN_NAME, &m_enemyBullets, SHOTGUN_FIRERATE, SHOTGUN_BULLETS_PER_SHOT, SHOTGUN_SPREAD,
					SHOTGUN_DAMAGE, SHOTGUN_BULLET_SPEED, SHOTGUN_RANGE, m_audioEngine.loadSoundEffect("Sounds/shotgun.wav"));
				shotgunEnemy->init(SHOTGUN_ENEMY_SPEED, m_spawners[i]->getPosition(), SHOTGUN_ENEMY_RANGE, SHOTGUN_ENEMY_DAMAGE,
					SHOTGUN_ENEMY_HEALTH, SHOTGUN_ENEMY_HEALTH_REGEN, SHOTGUN_ENEMY_SCORE, shotgunEnemyGun,
					GameEngine::ResourceManager::getTexture("Textures/enemyRed.png").id, m_audioEngine.loadSoundEffect("Sounds/pop.wav"));
				m_spawners[i]->enqueueEnemy(shotgunEnemy);
			}
		}
		// Bomb Enemies
		if (m_waves > BOMB_ENEMY_STARTSPAWNWAVE) {
			for (int j = BOMB_ENEMY_STARTSPAWNWAVE; j < m_waves; j++) {
				Enemy* bombEnemy = new Enemy;
				bombEnemy->init(BOMB_ENEMY_SPEED, m_spawners[i]->getPosition(), BOMB_ENEMY_RANGE, BOMB_ENEMY_DAMAGE,
					BOMB_ENEMY_HEALTH, BOMB_ENEMY_HEALTH_REGEN, BOMB_ENEMY_SCORE, NULL,
					GameEngine::ResourceManager::getTexture("Textures/bombBlackActive.png").id, m_audioEngine.loadSoundEffect("Sounds/pulse.wav"));
				m_spawners[i]->enqueueEnemy(bombEnemy);
			}
		}
	}
}

void MainGame::processInput() {

	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		// Pressing the X button in the top right corner
		case SDL_QUIT:
			m_gameState = GameState::EXIT;
			break;
		// Pressing a key passes the event to the input manager
		case SDL_KEYDOWN:
			m_inputManager.pressKey(event.key.keysym.sym);
			break;
		// Releasing a key passes the event to the input manager
		case SDL_KEYUP:
			m_inputManager.releaseKey(event.key.keysym.sym);
			break;
		// Pressing a mouse button passes the event to the input manager
		case SDL_MOUSEBUTTONDOWN:
			m_inputManager.pressKey(event.button.button);
			break;
		// Releasing a mouse button passes the event to the input manager
		case SDL_MOUSEBUTTONUP:
			m_inputManager.releaseKey(event.button.button);
			break;
		// Moving the mouse passes the coordinates to the input manager
		case SDL_MOUSEMOTION:
			m_inputManager.setMouseCoords((float)event.motion.x, (float)event.motion.y);
		}
	}
}

// Draws the game using OpenGL
void MainGame::drawGame() {

	// Set the base depth to 1.0
	glClearDepth(1.0);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_textureProgram.use();
	glActiveTexture(GL_TEXTURE0);
	GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	// Set the camera matrix
	GLuint pUniform = m_textureProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = m_camera.getCameraMatrix();

	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &(cameraMatrix[0][0]));


	// Draw level
	m_levels[m_currentLevel]->draw();

	m_agentSpriteBatch.begin();

	const glm::vec2 agentDims(AGENT_RADIUS * 2);

	// Draw humans
	for (unsigned int i = 0; i < m_humans.size(); i++) {
		if (m_camera.isBoxInView(m_humans[i]->getPosition(), agentDims)) {
			m_humans[i]->draw(m_agentSpriteBatch);
		}
	}
	// Draw enemies
	for (unsigned int i = 0; i < m_enemies.size(); i++) {
		if (m_camera.isBoxInView(m_enemies[i]->getPosition(), agentDims)) {
			m_enemies[i]->draw(m_agentSpriteBatch);
		}
	}

	// Draw bullets
	for (int i = 0; i < m_bullets.size(); i++) {
		if (m_camera.isBoxInView(m_bullets[i].getPosition(), agentDims)) {
			m_bullets[i].draw(m_agentSpriteBatch);
		}
	}

	// Draw enemy bullets
	for (int i = 0; i < m_enemyBullets.size(); i++) {
		if (m_camera.isBoxInView(m_enemyBullets[i].getPosition(), agentDims)) {
			m_enemyBullets[i].draw(m_agentSpriteBatch);
		}
	}
	m_agentSpriteBatch.end();
	m_agentSpriteBatch.renderBatch();

	m_particleEngine.draw(&m_agentSpriteBatch);

	drawHUD();

	m_textureProgram.unuse();

	// Swap buffer and draw everything to the screen
	m_window.swapBuffer();
}

void MainGame::drawHUD() {
	char buffer[256];

	GLuint pUniform = m_textureProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = m_staticCamera.getCameraMatrix();
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &(cameraMatrix[0][0]));

	m_hudSpriteBatch.begin();

	if (m_gameState != GameState::PLAY) {
		// Display gameover
		if (m_gameState == GameState::GAMEOVER) {
			if (m_victory) {
				sprintf_s(buffer, "You win!");
				m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(m_screenWidth / 2, m_screenHeight / 2 + FONT_RESOLUTION * 6),
					glm::vec2(1.0), 0.0f, GameEngine::ColorRGBA8(0, 255, 0, 255), GameEngine::Justification::MIDDLE);
			}
			else {
				sprintf_s(buffer, "You lose!");
				m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(m_screenWidth / 2, m_screenHeight / 2 + FONT_RESOLUTION * 6),
					glm::vec2(1.0), 0.0f, GameEngine::ColorRGBA8(255, 0, 0, 255), GameEngine::Justification::MIDDLE);
			}
		}
		// Display main menu and instructions
		else if (m_gameState == GameState::MENU) {
			if (m_instructionMenu) {
				sprintf_s(buffer, "Press SPACE to play or pause");
				m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(m_screenWidth / 2, m_screenHeight / 2 + FONT_RESOLUTION * 6),
					glm::vec2(1.0), 0.0f, GameEngine::ColorRGBA8(255, 255, 255, 255), GameEngine::Justification::MIDDLE);
				sprintf_s(buffer, "Use W, A, S, and D to move");
				m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(m_screenWidth / 2, m_screenHeight / 2 + FONT_RESOLUTION * 5),
					glm::vec2(1.0), 0.0f, GameEngine::ColorRGBA8(255, 255, 255, 255), GameEngine::Justification::MIDDLE);
				sprintf_s(buffer, "Use 1, 2, 3, and 4 to switch guns");
				m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(m_screenWidth / 2, m_screenHeight / 2 + FONT_RESOLUTION * 4),
					glm::vec2(1.0), 0.0f, GameEngine::ColorRGBA8(255, 255, 255, 255), GameEngine::Justification::MIDDLE);
				sprintf_s(buffer, "Use MOUSE to aim and LEFT CLICK to fire");
				m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(m_screenWidth / 2, m_screenHeight / 2 + FONT_RESOLUTION * 3),
					glm::vec2(1.0), 0.0f, GameEngine::ColorRGBA8(255, 255, 255, 255), GameEngine::Justification::MIDDLE);
				sprintf_s(buffer, "Press F to toggle fullscreen mode");
				m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(m_screenWidth / 2, m_screenHeight / 2 + FONT_RESOLUTION * 2),
					glm::vec2(1.0), 0.0f, GameEngine::ColorRGBA8(255, 255, 255, 255), GameEngine::Justification::MIDDLE);
			}
			else {
				sprintf_s(buffer, "Welcome to Gun Brawl");
				m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(m_screenWidth / 2, m_screenHeight / 2 + FONT_RESOLUTION * 6),
					glm::vec2(1.0), 0.0f, GameEngine::ColorRGBA8(255, 255, 255, 255), GameEngine::Justification::MIDDLE);
				sprintf_s(buffer, "Press SPACE to play or pause");
				m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(m_screenWidth / 2, m_screenHeight / 2 + FONT_RESOLUTION * 5),
					glm::vec2(1.0), 0.0f, GameEngine::ColorRGBA8(255, 255, 255, 255), GameEngine::Justification::MIDDLE);
				sprintf_s(buffer, "Press F to toggle fullscreen mode");
				m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(m_screenWidth / 2, m_screenHeight / 2 + FONT_RESOLUTION * 4),
					glm::vec2(1.0), 0.0f, GameEngine::ColorRGBA8(255, 255, 255, 255), GameEngine::Justification::MIDDLE);
				sprintf_s(buffer, "Press I for instructions");
				m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(m_screenWidth / 2, m_screenHeight / 2 + FONT_RESOLUTION * 3),
					glm::vec2(1.0), 0.0f, GameEngine::ColorRGBA8(255, 255, 255, 255), GameEngine::Justification::MIDDLE);
			}
		}
		sprintf_s(buffer, "Press ESCAPE to quit");
		m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(m_screenWidth / 2, m_screenHeight / 2 - FONT_RESOLUTION * 4),
			glm::vec2(1.0), 0.0f, GameEngine::ColorRGBA8(255, 255, 255, 255), GameEngine::Justification::MIDDLE);
	}
	// Health gets red as it approaches 0
	int healthColor = 256 * m_player->getHealth() / m_player->getMaxHealth() - 1;
	if (healthColor < 0) { healthColor = 0; }

	// Display HUD
	sprintf_s(buffer, "Health:");
	m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(0, m_screenHeight - FONT_RESOLUTION),
		glm::vec2(1.0), 0.0f, GameEngine::ColorRGBA8(255, 255, 255, 255), GameEngine::Justification::LEFT);

	sprintf_s(buffer, "%d", m_player->getHealth());
	m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(FONT_RESOLUTION * 3.5, m_screenHeight - FONT_RESOLUTION),
		glm::vec2(1.0), 0.0f, GameEngine::ColorRGBA8(255, healthColor, healthColor, 255), GameEngine::Justification::LEFT);

	sprintf_s(buffer, "Score: %d", m_score);
	m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(m_screenWidth / 2, m_screenHeight - FONT_RESOLUTION),
		glm::vec2(1.0), 0.0f, GameEngine::ColorRGBA8(255, 255, 255, 255), GameEngine::Justification::MIDDLE);

	sprintf_s(buffer, "Wave: %d", m_waves);
	m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(m_screenWidth, m_screenHeight - FONT_RESOLUTION),
		glm::vec2(1.0), 0.0f, GameEngine::ColorRGBA8(255, 255, 255, 255), GameEngine::Justification::RIGHT);

	// Gun selection and fade
	if (m_player->swappedGuns()) {
		m_gunSelectFade = 255;
	}
	m_gunSelectFade -= 3; // Rate of fading
	if (m_gunSelectFade > 0) {
		std::string curGun = m_player->getCurrentGun()->getName();
		sprintf_s(buffer, "%s", curGun.c_str());
		m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(0, 0),
			glm::vec2(1.0), 0.0f, GameEngine::ColorRGBA8(255, 255, 255, m_gunSelectFade), GameEngine::Justification::LEFT);
	}

	m_hudSpriteBatch.end();
	m_hudSpriteBatch.renderBatch();
}

void MainGame::updateCamera() {
	// Lock Camera to player or move independently
	if (m_inputManager.isKeyPressed(SDLK_y)) {
		m_lockedCam = !m_lockedCam;
	}
	if (m_lockedCam) {
		m_camera.setPosition(m_player->getPosition() + glm::vec2(AGENT_WIDTH / 2));
	}
	else {
		glm::vec2 mousePosition = m_inputManager.getMouseCoords();

		// Mouse Controls
		if (mousePosition.x < CAMERA_BORDER_SIZE) {
			m_camera.setPosition(glm::vec2(m_camera.getPosition().x - CAMERA_MOV_SPEED, m_camera.getPosition().y));
		}
		if (mousePosition.x > m_screenWidth - CAMERA_BORDER_SIZE) {
			m_camera.setPosition(glm::vec2(m_camera.getPosition().x + CAMERA_MOV_SPEED, m_camera.getPosition().y));
		}
		if (mousePosition.y < CAMERA_BORDER_SIZE) {
			m_camera.setPosition(glm::vec2(m_camera.getPosition().x, m_camera.getPosition().y + CAMERA_MOV_SPEED));
		}
		if (mousePosition.y > m_screenHeight - CAMERA_BORDER_SIZE) {
			m_camera.setPosition(glm::vec2(m_camera.getPosition().x, m_camera.getPosition().y - CAMERA_MOV_SPEED));
		}
	}

	// Zoom by changing scale
	if (m_inputManager.isKeyDown(SDLK_EQUALS) && m_camera.getScale() < m_screenScaleMin) {
		m_camera.setScale(m_camera.getScale() + CAMERA_SCALE_SPEED);
	}
	else if (m_inputManager.isKeyDown(SDLK_MINUS) && m_camera.getScale() > m_screenScaleMax) {
		m_camera.setScale(m_camera.getScale() - CAMERA_SCALE_SPEED);
	}
}
void MainGame::addBlood(const glm::vec2& position, int numParticles) {

	// Blood spreads at a random angle
	static std::mt19937 randEngine(time(nullptr));
	static std::uniform_real_distribution<float> randAngle(0.0f, 2 * M_PI);
	
	glm::vec2 velocity(2.0f, 0.1f);
	GameEngine::ColorRGBA8 color(255, 0, 0, 255);

	for (int i = 0; i < numParticles; i++) {
		m_bloodParticleBatch->addParticle(position, glm::rotate(velocity, randAngle(randEngine)), color, 10.0f);
	}
}