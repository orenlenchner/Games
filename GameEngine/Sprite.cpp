#include "Sprite.h"
#include "Vertex.h"
#include "ResourceManager.h"

#include <cstddef>

namespace GameEngine {

	Sprite::Sprite()
	{
		m_vboID = 0;
	}


	Sprite::~Sprite()
	{
		if (m_vboID != 0) {
			glDeleteBuffers(1, &m_vboID);
		}
	}

	void Sprite::init(float x, float y, float width, float height, std::string texturePath) {
		m_x = x;
		m_y = y;
		m_width = width;
		m_height = height;

		m_texture = ResourceManager::getTexture(texturePath);

		// Generate the buffer
		if (m_vboID == 0) {
			glGenBuffers(1, &m_vboID);
		}

		// Two triangles used to make a quad
		// Array that holds vertex data for two triangles
		// 6 vertices with an x and y coordinate each
		Vertex vertexData[6];

		// First Triangle
		vertexData[0].setPosition(x + width, y + width);
		vertexData[0].setUV(1.0f, 1.0f);

		vertexData[1].setPosition(x, y + width);
		vertexData[1].setUV(0.0f, 1.0f);

		vertexData[2].setPosition(x, y);
		vertexData[2].setUV(0.0f, 0.0f);

		// Second Triangle
		vertexData[3].setPosition(x, y);
		vertexData[3].setUV(0.0f, 0.0f);

		vertexData[4].setPosition(x + width, y);
		vertexData[4].setUV(1.0f, 0.0f);

		vertexData[5].setPosition(x + width, y + width);
		vertexData[5].setUV(1.0f, 1.0f);

		// Bind vertex buffer object using OpenGL
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

		// Unbind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// Draws the sprite to the screen
	void Sprite::draw() {

		glBindTexture(GL_TEXTURE_2D, m_texture.id);

		// Bind the VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

		// Enable the vertex attribute arrays
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		// Position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		// Color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		// UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		// Draw 6 vertices to the screen
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Disable the vertex attribute arrays
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Unbind the buffer object (VBO)
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}
}