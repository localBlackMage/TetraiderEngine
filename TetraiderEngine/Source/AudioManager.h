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
#define DEFAULT_VOL 0.5


// NOTES
// use Stream only when playing large music files, everything else should be loaded into memory
// BACKGROUND MUSIC FILES are loaded from HARD DRIVE
// SFX sounds are loaded from the MEMORY

typedef std::map<std::string, FMOD::Channel*> ChannelMap;


class AudioManager
{
public:
	AudioManager();
	~AudioManager();
	AudioManager(const AudioManager &) = delete;
	void operator=(const AudioManager &) = delete;

	void Update(float elapsed);
	void PlaySFX(const std::string& path, float volume,bool loop/*float minVol, float maxVol, float minPitch, float maxPitch*/);
	void PlaySFX(const std::string& path, float volume);
	void PlaySong(const std::string& path,float volume);
	void StopAllSFXs();
	void StopSFX(std::string& path);
	void StopSongs();
	void SetMasterVolume(float volume);
	void SetSFXsVolume(float volume);
	void SetSongsVolume(float volume);
	void ErrorCheck(FMOD_RESULT result);
	inline FMOD::System* getSystem() { return m_pSystem; }
	inline FMOD_MODE* getMode() { return m_Modes; }
	void TogglePause();
	bool isSoundPlaying(std::string);
private:

	FMOD::System* m_pSystem;
	FMOD::ChannelGroup* m_pMaster;
	FMOD::ChannelGroup* m_pGroups[2];
	ChannelMap m_Channel[2];
	FMOD_MODE m_Modes[2];

	FMOD::Channel* m_pCurrentSongChannel;
	std::string m_currentSongPath;
	std::string m_nextSongPath;
	std::string m_currentSfxPath;
	std::string m_nextSfxPath;

	enum FadeState{FADE_NONE,FADE_IN,FADE_OUT};
	FadeState m_fade;
	bool m_isChannelGroupPaused;
	bool m_isPlaying;
};


#endif