#include "AudioManager.h"
#include<stdlib.h>
#include<time.h>
#include <math.h>


float ChangeSemitone(float frequency, float variation)
{
	static float semitone_ratio = pow(2.0f, 1.0f / 12.0f);
	return frequency * pow(semitone_ratio, variation);
}
float ChangeOctave(float frequency, float variation)
{
	static float octave_ratio = 2.0f;
	return frequency * pow(octave_ratio, variation);
}
float RandomBetween(float min, float max)
{
	if (min == max)
		return min;
	float n = (float)rand() / (float)RAND_MAX;
	return min + n * (max - min);
}



AudioManager::AudioManager():currentSong(0),fade(FADE_NONE),isPaused(false)
{
	//initialize
	ErrorCheck(FMOD::System_Create(&system));
	ErrorCheck(system->init(100,FMOD_INIT_NORMAL,0));

	//create channel grp for each type of sound category
	ErrorCheck(system->getMasterChannelGroup(&master));
	for (int i = 0; i < CATEGORY_COUNT; i++)
	{
		ErrorCheck(system->createChannelGroup(0,&groups[i]));
		ErrorCheck(master->addGroup(groups[i]));
	}

	//setup modes for each category
	modes[SFX] = FMOD_DEFAULT ;
	modes[SONG] = FMOD_DEFAULT | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL;

	// seed value for SFx
	srand(0);
}

AudioManager::~AudioManager()
{
	//Release sound in each category
	SoundMap::iterator iter;
	for (int i=0;i<CATEGORY_COUNT;++i)
	{
		for (iter = sounds[i].begin(); iter != sounds[i].end(); ++iter)
		{
			iter->second->release();
		}
		sounds[i].clear();
	}
	//Release system
	ErrorCheck(system->release());
}

void AudioManager::Update(float elapsed)
{
	//in sec
	const float fadeTime = 1.0f;

	if (currentSong != 0 && fade == FADE_IN)
	{
		float volume;
		ErrorCheck(currentSong->getVolume(&volume));
		float nextVolume = volume + elapsed / fadeTime;
		if (nextVolume >= 1.0f)
		{
			ErrorCheck(currentSong->setVolume(1.0f));
			fade = FADE_NONE;
		}
		else
		{
			ErrorCheck(currentSong->setVolume(nextVolume));
		}
	}
	else if (currentSong!=0&&fade==FADE_OUT)
	{
		float volume;
		ErrorCheck(currentSong->getVolume(&volume));
		float nextVolume = volume + elapsed / fadeTime;
		if (nextVolume <= 0.0f)
		{
			ErrorCheck(currentSong->stop());
			currentSong = 0;
			currentSongPath.clear();
			fade = FADE_NONE;
		}
		else
		{
			ErrorCheck(currentSong->setVolume(nextVolume));
		}
	}
	else if (currentSong==0&&!nextSongPath.empty())
	{
		PlaySong(nextSongPath);
		nextSongPath.clear();
	}
	ErrorCheck(system->update());
}

void AudioManager::LoadSFX(const std::string & path)
{
	Load(SFX,path);
}

void AudioManager::LoadSong(const std::string & path)
{
	Load(SONG,path);
}

void AudioManager::PlaySFX(const std::string & path, float volume/*float minVol, float maxVol, float minPitch, float maxPitch*/)
{
	//if an sfx is already playing ignore
	/*if (currentSfxPath == path)
		return;*/


	// Try to find sound effect and return if not found 
	SoundMap::iterator sound = sounds[SFX].find(path);
	if(sound == sounds[SFX].end())
		return;
	
	//vol and pitch calculation
	/*float vol = RandomBetween(minVol,maxVol);
	float pitch= RandomBetween(minPitch, maxPitch);*/

	//check if sfx already playing
	

	//play sound with initial values
	//currentSfxPath = path;
	FMOD::Channel* channel=NULL;
	ErrorCheck(system->playSound(sound->second,NULL,true,&channel));
	ErrorCheck(channel->setChannelGroup(groups[SFX]));
	ErrorCheck(channel->setVolume(volume));

	/*float freq;
	channel->getFrequency(&freq);
	channel->setFrequency(ChangeSemitone(freq,pitch));*/
	ErrorCheck(channel->setPaused(false));
}



void AudioManager::PlaySong(const std::string & path)
{
	//ignore if song already playing
	if (currentSongPath==path)
		return;

	//if song is playing stop them and set this as next song
	if (currentSong != 0)
	{
		StopSongs();
		nextSongPath = path;
		return;
	}
	//find song in <-> sound map
	SoundMap::iterator sound = sounds[SONG].find(path);
	if (sound == sounds[SONG].end())
		return;

	//start playing song with 0 vol and fade in
	currentSongPath = path;
	ErrorCheck(system->playSound(sound->second, NULL, true, &currentSong));
	ErrorCheck(currentSong->setChannelGroup(groups[SONG]));
	ErrorCheck(currentSong->setVolume(0.0f));
	ErrorCheck(currentSong->setPaused(false));
	fade = FADE_IN;
}

void AudioManager::StopSFXs()
{
	ErrorCheck(groups[SFX]->stop());
}

void AudioManager::StopSongs()
{
	if (currentSong != 0)
		fade = FADE_OUT;
	nextSongPath.clear();
}

void AudioManager::SetMasterVolume(float volume)
{
	ErrorCheck(master->setVolume(volume));
}

void AudioManager::SetSFXsVolume(float volume)
{
	ErrorCheck(groups[SFX]->setVolume(volume));
}

void AudioManager::SetSongsVolume(float volume)
{
	ErrorCheck(groups[SONG]->setVolume(volume));
}

//void AudioManager::PauseAll()
//{
//	//pause all category of sounds
//	for (int i = 0; i < CATEGORY_COUNT; i++)
//		groups[i]->setPaused(true);
//}
//
//void AudioManager::ResumeAll()
//{
//	//pause all category of sounds
//	for (int i = 0; i < CATEGORY_COUNT; i++)
//		groups[i]->setPaused(false);
//}

void AudioManager::TogglePause()
{
	for (int i = 0; i < CATEGORY_COUNT; i++)
	{
		ErrorCheck(groups[i]->getPaused(&isPaused));
		if (isPaused)
			ErrorCheck(groups[i]->setPaused(false));
		else
			ErrorCheck(groups[i]->setPaused(true));
	}
}
void AudioManager::Load(Category type, const std::string & path)
{
	if (sounds[type].find(path) != sounds[type].end())
		return;
	FMOD::Sound* sound;
	 ErrorCheck(system->createSound(path.c_str(),modes[type],0,&sound));
	sounds[type].insert(std::make_pair(path, sound));
}

void AudioManager::ErrorCheck(FMOD_RESULT result) 
{
	if (result != FMOD_OK) {
		std::cout << FMOD_ErrorString(result) << std::endl;
	}
	return;
}