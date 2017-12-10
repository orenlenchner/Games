#include "ImageLoader.h"
#include "picopng.h"
#include "IOManager.h"
#include "EngineErrors.h"

namespace GameEngine {

	GLTexture ImageLoader::loadPNG(std::string filePath) {

		// Create an empty GLTexture
		GLTexture texture = {};

		// Input and output data
		std::vector<unsigned char> out;
		std::vector<unsigned char> in;

		unsigned long width, height;

		// Read the image file into buffer
		if (IOManager::readFileToBuffer(filePath, in) == false) {
			fatalError("Failed to load PNG file to buffer!");
		}

		// Decode the png format
		int errorCode = decodePNG(out, width, height, &(in[0]), in.size());
		if (errorCode != 0) {
			fatalError("decodePNG failed with error " + std::to_string(errorCode));
		}

		// Generate OpenGL texture object
		glGenTextures(1, &(texture.id));

		// Bind the texture object
		glBindTexture(GL_TEXTURE_2D, texture.id);

		// Upload the pixels to the texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

		// Set the textures parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// Generate mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);

		// Unbind the texture
		glBindTexture(GL_TEXTURE_2D, 0);

		texture.width = width;
		texture.height = height;

		// Return a copy of the texture data
		return texture;
	}
}