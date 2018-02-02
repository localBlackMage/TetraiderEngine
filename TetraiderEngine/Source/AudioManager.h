#pragma once
#ifndef _AUDIO_ENGINE_H_
#define _AUDIO_ENINGE_H_

#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "fmod_errors.h"
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <iostream>



// NOTES
// use Stream only when playing large music files, everything else should be loaded into memory
// BACKGROUND MUSIC FILES are loaded from HARD DRIVE
// SFX sounds are loaded from the MEMORY
class AudioManager
{
public:
	AudioManager();
	~AudioManager();
	AudioManager(const AudioManager &) = delete;
	void operator=(const AudioManager &) = delete;

	void Update(float elapsed);
	/*void LoadSFX(const std::string& path);
	void LoadSong(const std::string& path);*/
	void PlaySFX(const std::string& path, float volume/*float minVol, float maxVol, float minPitch, float maxPitch*/);
	void PlaySong(const std::string& path);
	void StopSFXs();
	void StopSongs();
	void SetMasterVolume(float volume);
	void SetSFXsVolume(float volume);
	void SetSongsVolume(float volume);
	void ErrorCheck(FMOD_RESULT result);
	inline FMOD::System* getSystem() { return m_pSystem; }
	inline FMOD_MODE* getMode() { return m_Modes; }
	/*void PauseAll();
	void ResumeAll();*/
	void TogglePause();
private:

	//typedef std::map<std::string, FMOD::Sound*> SoundMap;
	//typedef std::map<std::string, FMOD::Channel*> SoundChannelMap;
	enum Category{SFX,SONG,CATEGORY_COUNT};
	/*void Load(Category type, const std::string& path);*/

	

	FMOD::System* m_pSystem;
	FMOD::ChannelGroup* m_pMaster;
	FMOD::ChannelGroup* m_pGroups[CATEGORY_COUNT];
	//SoundChannelMap m_SoundChannel[CATEGORY_COUNT];
	//SoundMap m_Sounds[CATEGORY_COUNT];
	FMOD_MODE m_Modes[CATEGORY_COUNT];

	FMOD::Channel* m_pCurrentSongChannel;
	std::string m_currentSongPath;
	std::string m_nextSongPath;
	std::string m_currentSfxPath;
	std::string m_nextSfxPath;

	enum FadeState{FADE_NONE,FADE_IN,FADE_OUT};
	FadeState m_fade;
	bool m_isPaused;
	//bool m_isPlaying;
};


#endif