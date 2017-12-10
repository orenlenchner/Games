#pragma once

#include <SDL/SDL_mixer.h>
#include <string>
#include <map>

namespace GameEngine {

	class SoundEffect {
	public:
		friend class AudioEngine;

		// Plays the effect file
		// @param loops: If loops == -1, loop forever,
		// otherwise play it "loops"+1 times
		void play(int loops = 0);
		bool isNull() { return m_chunk == nullptr; }
	private:
		Mix_Chunk* m_chunk = nullptr;
	};

	class Music {
	public:
		friend class AudioEngine;

		// Plays the music file
		// @param loops: If loops == -1, loop forever,
		// otherwise play it "loops" times
		void play(int loops = 1);
		
		// Pauses the music
		static void pause();

		// Resumes the music
		static void resume();

		// Stops the music
		static void stop();
	private:
		Mix_Music* m_music = nullptr;
	};

	class AudioEngine
	{
	public:
		AudioEngine();
		~AudioEngine();

		void init();
		void destroy();

		SoundEffect loadSoundEffect(const std::string& filePath);

		Music loadMusic(const std::string& filePath);
	private:
		// Effects cache
		std::map<std::string, Mix_Chunk*> m_effectMap;
		
		// Music cache
		std::map<std::string, Mix_Music*> m_musicMap;

		bool m_isInitialized = false;
	};


}