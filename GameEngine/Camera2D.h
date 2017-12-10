#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace GameEngine {

	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();

		// Sets up the orthographic matrix and screen dimensions
		void init(int m_screenWidth, int m_screenHeight);

		// Updates the camera matrix if needed
		void update();

		// Updates the screen dimensions
		void Camera2D::updateDisplaySize(int screenWidth, int screenHeight);

		glm::vec2 convertScreenToWorld(glm::vec2 screenCoords);

		bool isBoxInView(const glm::vec2& position, const glm::vec2& dimensions);

		// Setters
		void setPosition(const glm::vec2& newPosition) { m_position = newPosition; m_needsMatrixUpdate = true; }
		void setScale(float newScale) { m_scale = newScale; m_needsMatrixUpdate = true; }

		// Getters
		glm::vec2 getPosition() { return m_position; }
		float getScale() { return m_scale; }
		glm::mat4 getCameraMatrix() { return m_cameraMatrix; }
		float getRelativeScale() { return m_screenHeight / m_screenWidth; }

	private:
		int m_screenWidth, m_screenHeight;
		bool m_needsMatrixUpdate;
		float m_scale;
		glm::vec2 m_position;
		glm::mat4 m_orthoMatrix;
		glm::mat4 m_cameraMatrix;
	};
}

