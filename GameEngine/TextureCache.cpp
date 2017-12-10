#include "TextureCache.h"
#include "ImageLoader.h"

namespace GameEngine {

	TextureCache::TextureCache()
	{
	}


	TextureCache::~TextureCache()
	{
	}


	GLTexture TextureCache::getTexture(std::string texturePath) {

		// Look up texture
		auto mit = m_textureMap.find(texturePath);

		// Check if texture is not in map
		if (mit == m_textureMap.end()) {
			// Load the texture
			GLTexture newTexture = ImageLoader::loadPNG(texturePath);

			// Insert new texture into map
			m_textureMap.insert(make_pair(texturePath, newTexture));

			return newTexture;
		}

		return mit->second;
	}
}