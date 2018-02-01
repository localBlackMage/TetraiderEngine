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
class AudioManager
{
public:

	AudioManager(const AudioManager &) = delete;
	void operator=(const AudioManager &) = delete;

	static AudioManager& GetInstance()
	{
		static AudioManager instance;
		return instance;
	}
	void Update(float elapsed);
	void LoadSFX(const std::string& path);
	void LoadSong(const std::string& path);
	void PlaySFX(const std::string& path, float volume/*float minVol, float maxVol, float minPitch, float maxPitch*/);
	void PlaySong(const std::string& path);
	void StopSFXs();
	void StopSongs();
	void SetMasterVolume(float volume);
	void SetSFXsVolume(float volume);
	void SetSongsVolume(float volume);
	bool isPlaying(int channelId);
	/*void PauseAll();
	void ResumeAll();*/
	void TogglePause();
private:
	AudioManager();
	~AudioManager();

	typedef std::map<std::string, FMOD::Sound*> SoundMap;
	typedef std::map<FMOD::Sound*, FMOD::Channel*> SFX_ChannelMap;
	enum Category{SFX,SONG,CATEGORY_COUNT};
	void Load(Category type, const std::string& path);

	void ErrorCheck(FMOD_RESULT result);

	FMOD::System* system;
	FMOD::ChannelGroup* master;
	FMOD::ChannelGroup* groups[CATEGORY_COUNT];
	SoundMap sounds[CATEGORY_COUNT];
	FMOD_MODE modes[CATEGORY_COUNT];

	FMOD::Channel* currentSong;
	std::string currentSongPath;
	std::string nextSongPath;
	std::string currentSfxPath;
	std::string nextSfxPath;

	enum FadeState{FADE_NONE,FADE_IN,FADE_OUT};
	FadeState fade;
	bool isPaused;
	
};


#endif