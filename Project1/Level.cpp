#include "Level.h"

#include <fstream>
#include <iostream>
#include <GameEngine/EngineErrors.h>
#include <GameEngine/ResourceManager.h>
#include <glm/glm.hpp>

Level::Level(const std::string& fileName) {
	std::ifstream file;
	file.open(fileName);

	// Error checking
	if (file.fail()) {
		GameEngine::fatalError("Failed to open " + fileName);
	}

	// Throw away the first string in temp
	std::string temp;
	file >> temp >> m_numHumans;
	std::getline(file, temp);

	while (std::getline(file, temp)) {
		m_levelData.push_back(temp);
	}

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	GameEngine::ColorRGBA8 whiteColor(255, 255, 255, 255);

	m_spriteBatch.init();
	m_spriteBatch.begin();
	
	// Place textures on tiles
	for (int y = 0; y < m_levelData.size(); y++) {
		for (int x = 0; x < m_levelData[y].size(); x++) {
			char tile = m_levelData[y][x];

			glm::vec4 destRec(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			switch (tile) {
			case 'R':
				m_spriteBatch.draw(destRec, uvRect, GameEngine::ResourceManager::getTexture("Textures/walls.png").id, 0.0f, whiteColor);
				break;
			case '.':
				m_spriteBatch.draw(destRec, uvRect, GameEngine::ResourceManager::getTexture("Textures/ground.png").id, 0.0f, whiteColor);
				break;
			case '@':
				m_startPlayerPos.x = x * TILE_WIDTH;
				m_startPlayerPos.y = y * TILE_WIDTH;
				m_levelData[y][x] = '.';
				m_spriteBatch.draw(destRec, uvRect, GameEngine::ResourceManager::getTexture("Textures/ground.png").id, 0.0f, whiteColor);
				break;
			case 'Z':
				m_enemyStartPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				m_enemySpawnPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				m_levelData[y][x] = '.';
				m_spriteBatch.draw(destRec, uvRect, GameEngine::ResourceManager::getTexture("Textures/ground.png").id, 0.0f, whiteColor);
				break;
			default:
				std::printf("Unexpected symbol %c at (%d,%d)", tile, x, y);
				break;
			}
		}
	}
	m_spriteBatch.end();
}

Level::~Level()
{
}

void Level::draw() {
	m_spriteBatch.renderBatch();
}