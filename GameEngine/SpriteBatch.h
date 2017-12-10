#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "Vertex.h"

namespace GameEngine {

	// Determines how glyphs are sorted
	enum class GlyphSortType {
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};

	// Glyphs are single quads
	class Glyph {
	public:
		Glyph() {};
		Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color);
		Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color, float angle);

		GLuint texture;
		float depth;

		Vertex topLeft;
		Vertex bottomLeft;
		Vertex topRight;
		Vertex bottomRight;
	private:
		// Rotates a point about (0,0) by an angle
		glm::vec2 rotatePoint(glm::vec2& pos, float angle);
	};

	// Render batches are used for single draw calls
	class RenderBatch {
	public:
		RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) :
			offset(Offset), numVertices(NumVertices), texture(Texture)
		{
		}
		GLuint offset;
		GLuint numVertices;
		GLuint texture;
	};

	// More efficient using sprite batch to draw sprites than drawing them individually
	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();

		// Initializes the spritebatch
		void init();

		// Begins the spritebatch
		void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);

		// Ends the spritebatch
		void end();

		// Adds a glyph to the spritebatch
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color);

		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, float angle);
		
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, const glm::vec2& dir);

		// Renders the entire spritebatch to the screen
		void renderBatch();

	private:
		// Creates all the needed RenderBatches
		void createRenderBatches();

		// Generates VAO and VBO
		void createVertexArray();

		// Sorts glyphs according to the sortType
		void sortGlyphs();

		static bool compareFrontToBack(Glyph* a, Glyph* b);
		static bool compareBackToFront(Glyph* a, Glyph* b);
		static bool compareTexture(Glyph* a, Glyph* b);

		GLuint m_vbo;
		GLuint m_vao;

		GlyphSortType m_sortType;

		std::vector<Glyph*> m_glyphPtrs; // For sorting
		std::vector<Glyph> m_glyphs; // Actual glyphs
		std::vector<RenderBatch> m_renderBatches;
	};

}