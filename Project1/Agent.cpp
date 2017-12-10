#include "Agent.h"
#include <GameEngine/ResourceManager.h>
#include "Level.h"
#include <algorithm>
#include <iostream>

Agent::Agent()
{
}


Agent::~Agent()
{
}

bool Agent::collideWithLevel(const std::vector<std::string>& levelData) {
	std::vector<glm::vec2> collideTilePosition;

	// Check the four corners
	// First corner
	checkTilePosition(levelData, collideTilePosition, m_position.x, m_position.y);
	// Second corner
	checkTilePosition(levelData, collideTilePosition, m_position.x + AGENT_WIDTH, m_position.y);
	// Third corner
	checkTilePosition(levelData, collideTilePosition, m_position.x, m_position.y + AGENT_WIDTH);
	// Forth corner
	checkTilePosition(levelData, collideTilePosition, m_position.x + AGENT_WIDTH, m_position.y + AGENT_WIDTH);

	if (collideTilePosition.size() == 0) {
		return false;
	}

	/* Sorts the four tiles so the closest one to the agent will be updated first
	std::sort(collideTilePosition.begin(), collideTilePosition.end(),
		[this](const glm::vec2 lhs, const glm::vec2 rhs) {
			float lDist = sqrt((lhs.x - m_position.x) * (lhs.x - m_position.x) + (lhs.y - m_position.y) * (lhs.y - m_position.y));
			float rDist = sqrt((rhs.x - m_position.x) * (rhs.x - m_position.x) + (rhs.y - m_position.y) * (rhs.y - m_position.y));
			return lDist > rDist;
		}
	);*/

	for (unsigned int i = 0; i < collideTilePosition.size(); i++) {
		collideWithTile(collideTilePosition[i]);
	}
	return true;
}

bool Agent::collideWithAgent(Agent* agent) {
	
	// Collision distance between agents
	const float MIN_DISTANCE = AGENT_RADIUS * 2.0f;

	// This agent's center position
	glm::vec2 centerPosA = m_position + glm::vec2(AGENT_RADIUS);

	// Other agent's center position
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	// Distance between agents
	glm::vec2 distVec = centerPosA - centerPosB;
	float distance = glm::length(distVec);

	// If the distance is greater than the minimum distance, then there is a collision
	float collisionDepth = MIN_DISTANCE - distance;
	if (collisionDepth > 0 && collisionDepth < MIN_DISTANCE) {
		// Calculates how deep the collision is
		glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth;
		// Moves both agents back by the depth of the collision
		m_position += collisionDepthVec / 2.0f;
		agent->m_position -= collisionDepthVec / 2.0f;

		return true;
	}
	return false;
}

void Agent::draw(GameEngine::SpriteBatch& m_spriteBatch) {

	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 destRect;
	// Get agent's corners
	destRect.x = m_position.x;
	destRect.y = m_position.y;
	destRect.z = AGENT_WIDTH;
	destRect.w = AGENT_WIDTH;

	// Draw agent
	m_spriteBatch.draw(destRect, uvRect, m_textureID, 0.0f, m_color, m_direction);
}

bool Agent::applyDamage(float damage) {
	// Deal damage
	m_health -= damage;
	// Check if agent died
	if (m_health <= 0) {
		return true;
	}
	return false;
}

void Agent::checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePosition, float x, float y) {
	
	// Get the position of this corner in the grid
	glm::vec2 cornerPos = glm::vec2(floor(x / (float)TILE_WIDTH), floor(y / (float)TILE_WIDTH));

	// Return if outside world
	if (cornerPos.x < 1 || cornerPos.x >= levelData[0].size() - 1 ||
		cornerPos.y < 1 || cornerPos.y >= levelData.size() - 1) {
		m_position.x = levelData[0].size() / 2 * TILE_WIDTH;
		m_position.y = levelData.size() / 2 * TILE_WIDTH;
		return;
	}

	// If not in air tile, collide with tile
	if (levelData[cornerPos.y][cornerPos.x] != '.') {
		collideTilePosition.push_back(cornerPos * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2.0f));
	}
}

// Axis Aligned Bounding Box Collision
void Agent::collideWithTile(glm::vec2 tilePos) {

	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;

	// Collision distance between an agent and a tile
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

	// Agent center position
	glm::vec2 centerPlayerPos = m_position + glm::vec2(AGENT_RADIUS);

	// Distance vector from the agent center to the tile position
	glm::vec2 distVec = centerPlayerPos - tilePos;

	// Calculates how deep the collision is
	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);

	// Collide if both depths are greater than zero
	if (xDepth > 0 && yDepth > 0) {

		// Check which direction has the greater collision depth
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)) {
			// X collision depth is lesser, move in X direction
			if (distVec.x < 0) {
				// Colliding right
				m_position.x -= xDepth;
			}
			else {
				// Colliding left
				m_position.x += xDepth;
			}
		}
		else {
			// Y collision depth is lesser, move in Y direction
			if (distVec.y < 0) {
				// Colliding up
				m_position.y -= yDepth;
			}
			else {
				// Colliding down
				m_position.y += yDepth;
			}
		}
	}
}

void Agent::regenHealth(float healthRegen) {
	if (count == HEALTH_REGEN_RATE) {
		m_health += healthRegen;
		if (m_health > m_maxHealth) {
			m_health = m_maxHealth;
		}
		count = 0;
	}
	count++;
}