#include "SpriteBatch.h"
#include <algorithm>

namespace GameEngine {

	Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color) :
		texture(Texture), depth(Depth) {

		// Set the attributes of the four corners of the glyph
		topLeft.color = color;
		topLeft.setPosition(destRect.x, destRect.y + destRect.w);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		bottomLeft.color = color;
		bottomLeft.setPosition(destRect.x, destRect.y);
		bottomLeft.setUV(uvRect.x, uvRect.y);

		bottomRight.color = color;
		bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
		bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		topRight.color = color;
		topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	}

	Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color, float angle) :
		texture(Texture),
		depth(Depth) {

		glm::vec2 halfDims(destRect.z / 2.0f, destRect.w / 2.0f);

		// Get points centered at (0,0)
		glm::vec2 tl(-halfDims.x, halfDims.y);
		glm::vec2 bl(-halfDims.x, -halfDims.y);
		glm::vec2 br(halfDims.x, -halfDims.y);
		glm::vec2 tr(halfDims.x, halfDims.y);

		// Rotate the points
		tl = rotatePoint(tl, angle) + halfDims;
		bl = rotatePoint(bl, angle) + halfDims;
		br = rotatePoint(br, angle) + halfDims;
		tr = rotatePoint(tr, angle) + halfDims;

		// Set the attributes of the four corners of the glyph
		topLeft.color = color;
		topLeft.setPosition(destRect.x + tl.x, destRect.y + tl.y);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		bottomLeft.color = color;
		bottomLeft.setPosition(destRect.x + bl.x, destRect.y + bl.y);
		bottomLeft.setUV(uvRect.x, uvRect.y);

		bottomRight.color = color;
		bottomRight.setPosition(destRect.x + br.x, destRect.y + br.y);
		bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		topRight.color = color;
		topRight.setPosition(destRect.x + tr.x, destRect.y + tr.y);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	}

	glm::vec2 Glyph::rotatePoint(glm::vec2& pos, float angle) {
		glm::vec2 vec;
		vec.x = pos.x * cos(angle) - pos.y * sin(angle);
		vec.y = pos.x * sin(angle) + pos.y * cos(angle);
		return vec;
	}

	SpriteBatch::SpriteBatch() : m_vbo(0), m_vao(0)
	{
	}

	SpriteBatch::~SpriteBatch()
	{
	}

	void SpriteBatch::init() {
		createVertexArray();
	}

	void SpriteBatch::begin(GlyphSortType sortType /* GlyphSortType::TEXTURE */) {
		m_sortType = sortType;
		m_renderBatches.clear();
		// Sets m_glyphs.size == 0, but does not free memory
		m_glyphs.clear();
	}

	void SpriteBatch::end() {
		// Set up pointers for sorting
		m_glyphPtrs.resize(m_glyphs.size());
		for (int i = 0; i < m_glyphs.size(); i++) {
			m_glyphPtrs[i] = &m_glyphs[i];
		}
		sortGlyphs();
		createRenderBatches();
	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color) {
		m_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, float angle) {
		m_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, const glm::vec2& dir) {
		
		const glm::vec2 right(1.0f, 0.0f);
		float angle = acos(glm::dot(right, dir));
		if (dir.y < 0.0f) angle = -angle;
		m_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);

	}
	void SpriteBatch::renderBatch() {

		// Bind VAO
		glBindVertexArray(m_vao);

		for (unsigned int i = 0; i < m_renderBatches.size(); i++) {
			glBindTexture(GL_TEXTURE_2D, m_renderBatches[i].texture);
			glDrawArrays(GL_TRIANGLES, m_renderBatches[i].offset, m_renderBatches[i].numVertices);
		}

		glBindVertexArray(0);
	}

	void SpriteBatch::createRenderBatches() {
		// Vector for the vertices that will be used
		std::vector<Vertex> vertices;
		
		// Resize vector
		vertices.resize(m_glyphs.size() * 6);

		if (m_glyphs.empty()) {
			return;
		}

		// Add the first glyph to the batch
		int offset = 0;
		int cv = 0; // current vertex
		m_renderBatches.emplace_back(offset, 6, m_glyphPtrs[0]->texture);
		vertices[cv++] = m_glyphPtrs[0]->topLeft;
		vertices[cv++] = m_glyphPtrs[0]->bottomLeft;
		vertices[cv++] = m_glyphPtrs[0]->bottomRight;
		vertices[cv++] = m_glyphPtrs[0]->bottomRight;
		vertices[cv++] = m_glyphPtrs[0]->topRight;
		vertices[cv++] = m_glyphPtrs[0]->topLeft;
		offset += 6;

		// Add the other glyphs
		for (unsigned int cg = 1 ; cg < m_glyphPtrs.size(); cg++) { // cg = current glyph
			// Check if glyph can be added to the current batch
			if (m_glyphPtrs[cg]->texture != m_glyphPtrs[cg - 1]->texture) {
				// Make a new batch
				m_renderBatches.emplace_back(offset, 6, m_glyphPtrs[cg]->texture);
			}
			else {
				// Add it to the current batch by updating numVertices
				m_renderBatches.back().numVertices += 6;
			}
			vertices[cv++] = m_glyphPtrs[cg]->topLeft;
			vertices[cv++] = m_glyphPtrs[cg]->bottomLeft;
			vertices[cv++] = m_glyphPtrs[cg]->bottomRight;
			vertices[cv++] = m_glyphPtrs[cg]->bottomRight;
			vertices[cv++] = m_glyphPtrs[cg]->topRight;
			vertices[cv++] = m_glyphPtrs[cg]->topLeft;
			offset += 6;
		}

		// Bind buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		// Orphan the buffer
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		// Upload the buffer
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
		// Unbind buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}
	void SpriteBatch::createVertexArray() {
		
		// Generate VAO
		if (m_vao == 0) {
			glGenVertexArrays(1, &m_vao);
		}

		// Bind VAO
		glBindVertexArray(m_vao);
		
		// Generate the VBO
		if (m_vbo == 0) {
			glGenBuffers(1, &m_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

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

		glBindVertexArray(0);

	}

	void SpriteBatch::sortGlyphs() {
		switch (m_sortType) {
		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(m_glyphPtrs.begin(), m_glyphPtrs.end(), compareFrontToBack);
			break;
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(m_glyphPtrs.begin(), m_glyphPtrs.end(), compareBackToFront);
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(m_glyphPtrs.begin(), m_glyphPtrs.end(), compareTexture);
			break;
		}
	}

	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b) {
		return (a->depth < b->depth);
	}
	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b) {
		return (a->depth > b->depth);
	}
	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b) {
		return (a->texture < b->texture);
	}

}
