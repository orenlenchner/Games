#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

namespace GameEngine {

	/* Input manager stores a key map that maps SDL_Keys to the booleans.
	 * If the value in the key map is true, then the key is down.
	 */
	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void update();

		// Marks a key as pressed
		void pressKey(unsigned int keyID);

		// Marks a key as not pressed
		void releaseKey(unsigned int keyID);

		// Sets the mouse coordinates
		void setMouseCoords(float x, float y);

		// Returns true if the key is currently down
		bool isKeyDown(unsigned int keyID);

		// Returns true if the key was just pressed this frame
		bool isKeyPressed(unsigned int keyID);

		// Return true if any key is being pressed down
		bool InputManager::noKeyPressed();

		// Getters
		glm::vec2 getMouseCoords() const { return m_mouseCoords; }
	private:

		bool wasKeyDown(unsigned int keyID);
		std::unordered_map<unsigned int, bool> m_keyMap;
		std::unordered_map<unsigned int, bool> m_prevKeyMap;
		glm::vec2 m_mouseCoords;
	};
}