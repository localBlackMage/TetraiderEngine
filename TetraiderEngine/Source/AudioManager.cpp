#include "AudioManager.h"
#include <stdlib.h>
#include <time.h>
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



AudioManager::AudioManager():m_pCurrentSongChannel(0), m_fade(FADE_NONE), m_isPaused(false)
{
	//initialize
	ErrorCheck(FMOD::System_Create(&m_pSystem));
	ErrorCheck(m_pSystem->init(100,FMOD_INIT_NORMAL,0));

	//create channel grp for each type of sound category
	ErrorCheck(m_pSystem->getMasterChannelGroup(&m_pMaster));
	for (int i = 0; i < CATEGORY_COUNT; i++)
	{
		ErrorCheck(m_pSystem->createChannelGroup(0,&m_pGroups[i]));
		ErrorCheck(m_pMaster->addGroup(m_pGroups[i]));
	}

	//setup modes for each category
	m_Modes[SFX] = FMOD_DEFAULT ;
	m_Modes[SONG] = FMOD_DEFAULT | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL;

	// seed value for SFx
	srand(0);
}

AudioManager::~AudioManager()
{
	//Release sound in each category
	SoundMap::iterator iter;
	for (int i=0;i<CATEGORY_COUNT;++i)
	{
		for (iter = m_Sounds[i].begin(); iter != m_Sounds[i].end(); ++iter)
		{
			iter->second->release();
		}
		m_Sounds[i].clear();
	}
	//Release system
	ErrorCheck(m_pSystem->release());
}

void AudioManager::Update(float elapsed)
{
	//in sec
	const float fadeTime = 1.0f;

	if (m_pCurrentSongChannel != 0 && m_fade == FADE_IN)
	{
		float volume;
		ErrorCheck(m_pCurrentSongChannel->getVolume(&volume));
		float nextVolume = volume + elapsed / fadeTime;
		if (nextVolume >= 1.0f)
		{
			ErrorCheck(m_pCurrentSongChannel->setVolume(1.0f));
			m_fade = FADE_NONE;
		}
		else
		{
			ErrorCheck(m_pCurrentSongChannel->setVolume(nextVolume));
		}
	}
	else if (m_pCurrentSongChannel !=0&& m_fade ==FADE_OUT)
	{
		float volume;
		ErrorCheck(m_pCurrentSongChannel->getVolume(&volume));
		float nextVolume = volume + elapsed / fadeTime;
		if (nextVolume <= 0.0f)
		{
			ErrorCheck(m_pCurrentSongChannel->stop());
			m_pCurrentSongChannel = 0;
			m_currentSongPath.clear();
			m_fade = FADE_NONE;
		}
		else
		{
			ErrorCheck(m_pCurrentSongChannel->setVolume(nextVolume));
		}
	}
	else if (m_pCurrentSongChannel ==0&&!m_nextSongPath.empty())
	{
		PlaySong(m_nextSongPath);
		m_nextSongPath.clear();
	}
	ErrorCheck(m_pSystem->update());
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
	SoundMap::iterator sound = m_Sounds[SFX].find(path);
	if(sound == m_Sounds[SFX].end())
		return;
	
	//vol and pitch calculation
	/*float vol = RandomBetween(minVol,maxVol);
	float pitch= RandomBetween(minPitch, maxPitch);*/

	//check if sfx already playing
	

	//play sound with initial values
	//currentSfxPath = path;
	FMOD::Channel* channel=NULL;
	ErrorCheck(m_pSystem->playSound(sound->second,NULL,true,&channel));
	ErrorCheck(channel->setChannelGroup(m_pGroups[SFX]));
	ErrorCheck(channel->setVolume(volume));

	/*float freq;
	channel->getFrequency(&freq);
	channel->setFrequency(ChangeSemitone(freq,pitch));*/
	ErrorCheck(channel->setPaused(false));
}



void AudioManager::PlaySong(const std::string & path)
{
	//ignore if song already playing
	if (m_currentSongPath ==path)
		return;

	//if song is playing stop them and set this as next song
	if (m_pCurrentSongChannel != 0)
	{
		StopSongs();
		m_nextSongPath = path;
		return;
	}
	//find song in <-> sound map
	SoundMap::iterator sound = m_Sounds[SONG].find(path);
	if (sound == m_Sounds[SONG].end())
		return;

	//start playing song with 0 vol and fade in
	m_currentSongPath = path;
	ErrorCheck(m_pSystem->playSound(sound->second, NULL, true, &m_pCurrentSongChannel));
	ErrorCheck(m_pCurrentSongChannel->setChannelGroup(m_pGroups[SONG]));
	ErrorCheck(m_pCurrentSongChannel->setVolume(0.0f));
	ErrorCheck(m_pCurrentSongChannel->setPaused(false));
	m_fade = FADE_IN;
}

void AudioManager::StopSFXs()
{
	ErrorCheck(m_pGroups[SFX]->stop());
}

void AudioManager::StopSongs()
{
	if (m_pCurrentSongChannel != 0)
		m_fade = FADE_OUT;
	m_nextSongPath.clear();
}

void AudioManager::SetMasterVolume(float volume)
{
	ErrorCheck(m_pMaster->setVolume(volume));
}

void AudioManager::SetSFXsVolume(float volume)
{
	ErrorCheck(m_pGroups[SFX]->setVolume(volume));
}

void AudioManager::SetSongsVolume(float volume)
{
	ErrorCheck(m_pGroups[SONG]->setVolume(volume));
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
		ErrorCheck(m_pGroups[i]->getPaused(&m_isPaused));
		if (m_isPaused)
			ErrorCheck(m_pGroups[i]->setPaused(false));
		else
			ErrorCheck(m_pGroups[i]->setPaused(true));
	}
}
void AudioManager::Load(Category type, const std::string & path)
{
	if (m_Sounds[type].find(path) != m_Sounds[type].end())
		return;
	FMOD::Sound* sound;
	 ErrorCheck(m_pSystem->createSound(path.c_str(), m_Modes[type],0,&sound));
	 m_Sounds[type].insert(std::make_pair(path, sound));
}

void AudioManager::ErrorCheck(FMOD_RESULT result) 
{
	if (result != FMOD_OK) {
		std::cout << FMOD_ErrorString(result) << std::endl;
	}
	return;
}