#pragma once

#include <vector>

namespace GameEngine {

	class IOManager
	{
	public:
		// Return true if file is sucessfully stored in the buffer
		static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);
	};
}