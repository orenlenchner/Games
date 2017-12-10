#pragma once

#include <string>
#include <vector>
#include <GameEngine/SpriteBatch.h>

const float TILE_WIDTH = 64;

class Level
{
public:
	// Load the level
	Level(const std::string& fileName);
	~Level();

	// Draw the level
	void draw();

	// Getters
	// Return the tile width of the level
	int getWidth() const { return m_levelData[0].size(); }

	// Return the tile height of the level
	int getHeight() const { return m_levelData.size(); }

	// Return the total number of humans (including the player)
	int getNumHumans() const { return m_numHumans; }

	// Returns the levels data as a vector of strings
	const std::vector<std::string>& getLevelData() const { return m_levelData; }

	// Returns the players start position in the level
	glm::vec2 getStartPlayerPos() const { return m_startPlayerPos;  }

	// Returns the start positions of the initial enemies (if there are any) and spawner positions
	const std::vector<glm::vec2>& getStartEnemyPos() const { return m_enemyStartPositions; }
	const std::vector<glm::vec2>& getEnemySpawnPos() const { return m_enemySpawnPositions; }

private:
	// Level values
	std::vector<std::string> m_levelData;
	GameEngine::SpriteBatch m_spriteBatch;
	int m_numHumans;
	glm::vec2 m_startPlayerPos;
	std::vector<glm::vec2> m_enemyStartPositions;
	std::vector<glm::vec2> m_enemySpawnPositions;
};

