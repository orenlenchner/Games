#include "AudioEngine.h"
#include "EngineErrors.h"

namespace GameEngine {

	void SoundEffect::play(int loops /* = 0 */) {
		if (Mix_PlayChannel(-1, m_chunk, loops) == -1) {
			if (Mix_PlayChannel(0, m_chunk, loops) == -1) {
				fatalError("Mix_PlayChannel error: " + std::string(Mix_GetError()));
			}
		}
	}

	void Music::play(int loops /* = -1 */) {
		if (Mix_PlayMusic( m_music, loops) == -1) {
			fatalError("Mix_PlayMusic error: " + std::string(Mix_GetError()));
		}
	}

	void Music::pause() {
		Mix_PauseMusic();
	}
	void Music::stop() {
		Mix_HaltMusic();
	}
	void Music::resume() {
		Mix_ResumeMusic();
	}

	AudioEngine::AudioEngine() {
		// Empty
	}


	AudioEngine::~AudioEngine() {
		destroy();
	}

	void AudioEngine::init() {
		if (m_isInitialized) {
			fatalError("Tried to initialize Audio Engine twice");
		}
		// Parameter can be a bitwose combination of MIX_INIT_FAC, MIX_INIT_MOD, MIX_INIT_MP3, MIX_INIT_OGG
		if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1) {
			fatalError("Mix_Init error: " + std::string(Mix_GetError()));
		}

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
			fatalError("Mix_Init error: " + std::string(Mix_GetError()));
		}

		m_isInitialized = true;
	}
	void AudioEngine::destroy() {
		if (m_isInitialized) {
			m_isInitialized = false;

			for (auto& it : m_effectMap) {
				Mix_FreeChunk(it.second);
			}
			for (auto& it : m_musicMap) {
				Mix_FreeMusic(it.second);
			}

			m_effectMap.clear();
			m_musicMap.clear();

			Mix_CloseAudio();
			Mix_Quit();
		}
	}

	SoundEffect AudioEngine::loadSoundEffect(const std::string& filePath) {
		// Try to find existing audio
		auto it = m_effectMap.find(filePath);

		SoundEffect effect;

		if (it == m_effectMap.end()) {
			// Not cached
			Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
			// Check if chunk exists
			if (chunk == nullptr) {
				fatalError("Mix_LoadWAV error: " + std::string(Mix_GetError()));
			}
			// Cache sound
			effect.m_chunk = chunk;
			m_effectMap[filePath] = chunk;
		}
		else {
			// Already cached
			effect.m_chunk = it->second;
		}
		
		return effect;
	}
	Music AudioEngine::loadMusic(const std::string& filePath) {
		// Try to find existing audio
		auto it = m_musicMap.find(filePath);

		Music music;

		if (it == m_musicMap.end()) {
			// Not cached
			Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());
			// Check if mix exists
			if (mixMusic == nullptr) {
				fatalError("Mix_LoadMUS error: " + std::string(Mix_GetError()));
			}
			// Cache music
			music.m_music = mixMusic;
			m_musicMap[filePath] = mixMusic;
		}
		else {
			// Already cached
			music.m_music = it->second;
		}

		return music;
	}
}