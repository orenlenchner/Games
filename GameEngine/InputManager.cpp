#include "InputManager.h"
//#include <SDL/SDL.h>

namespace GameEngine {

	InputManager::InputManager() : m_mouseCoords(0)
	{
	}


	InputManager::~InputManager()
	{
	}

	void InputManager::update() {
		// Loop through m_keyMap using for each loop, and copy it over to m_prevKeyMap
		for (auto& it : m_keyMap) {
			m_prevKeyMap[it.first] = it.second;
		}
	}

	void InputManager::pressKey(unsigned int keyID) {
		m_keyMap[keyID] = true;
	}
	void InputManager::releaseKey(unsigned int keyID) {
		m_keyMap[keyID] = false;
	}

	void InputManager::setMouseCoords(float x, float y) {
		m_mouseCoords.x = x;
		m_mouseCoords.y = y;
	}

	bool InputManager::isKeyDown(unsigned int keyID) {
		// Search for the key to check if it is down
		auto it = m_keyMap.find(keyID);
		if (it != m_keyMap.end()) {
			return it->second;
		}
		return false;
	}

	bool InputManager::isKeyPressed(unsigned int keyID) {
		if (isKeyDown(keyID) && !wasKeyDown(keyID)) {
			return true;
		}
		return false;
	}

	bool InputManager::wasKeyDown(unsigned int keyID) {
		// Search for the key in the previous key map to see if it was just down
		auto it = m_prevKeyMap.find(keyID);
		if (it != m_prevKeyMap.end()) {
			return it->second;
		}
		return false;
	}

	bool InputManager::noKeyPressed() {
		return m_keyMap.empty();
	}
}