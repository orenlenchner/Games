#pragma once
#include "TextureCache.h"
#include <string>

namespace GameEngine {

	// To access all resources: textures
	class ResourceManager
	{
	public:
		static GLTexture getTexture(std::string texturePath);

	private:
		static TextureCache m_textureCache;
	};
}
