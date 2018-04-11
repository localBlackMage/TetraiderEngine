//#include "AudioManager.h"
//#include <stdlib.h>
//#include <time.h>
//#include <math.h>
//#include "Event.h"
//#include "TetraiderAPI.h"
//#include "Transform.h"

#include <Stdafx.h>

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

AudioManager::AudioManager() :m_pCurrentSongChannel(0), m_fade(FADE_NONE), m_isChannelGroupPaused(false)
{
	//initialize
	ErrorCheck(FMOD::System_Create(&m_pSystem));
	ErrorCheck(m_pSystem->init(100, FMOD_INIT_NORMAL, 0));

	//create channel grp for each type of sound category
	ErrorCheck(m_pSystem->getMasterChannelGroup(&m_pMaster));
	for (int i = 0; i < CATEGORY_COUNT; i++)
	{
		ErrorCheck(m_pSystem->createChannelGroup(0, &m_pGroups[i]));
		ErrorCheck(m_pMaster->addGroup(m_pGroups[i]));
	}

	//setup modes for each category
	m_Modes[SFX] = FMOD_DEFAULT /*|FMOD_3D*/;
	m_Modes[SONG] = FMOD_DEFAULT | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL /*| FMOD_3D*/;

	//init fade time
	m_fadeTime = 100.0f;
	// seed value for SFx
	srand(0);
	TETRA_EVENTS.Subscribe(EVENT_INPUT_PAUSEMUSIC, this);
	TETRA_EVENTS.Subscribe(EVENT_ChangeBGMVol,this);
	TETRA_EVENTS.Subscribe(EVENT_ChangeSFXVol, this);
	TETRA_EVENTS.Subscribe(EVENT_ChangeMasterVol, this);
}

AudioManager::~AudioManager()
{

}

void AudioManager::Update(float elapsed)
{
	float vol;
	m_pCurrentSongChannel->getVolume(&vol);
	ScaledVol(vol);

	if (TETRA_GAME_CONFIG.GetsoundsMute())
	{
		SetMasterVolume(0.0);
	}

	if (m_pCurrentSongChannel != 0 && m_fade == FADE_IN)
	{
		float volume;
		ErrorCheck(m_pCurrentSongChannel->getVolume(&volume));
		float nextVolume = volume + elapsed / m_fadeTime;
		if (nextVolume >= ScaledVol(m_musicVol))
		{
			ErrorCheck(m_pCurrentSongChannel->setVolume(m_musicVol));
			m_fade = FADE_NONE;
		}
		else
		{
			ErrorCheck(m_pCurrentSongChannel->setVolume(nextVolume));
		}
	}
	else if (m_pCurrentSongChannel != 0 && m_fade==FADE_OUT)
	{
		float volume;
		ErrorCheck(m_pCurrentSongChannel->getVolume(&volume));
		float nextVolume = volume - elapsed / m_fadeTime;
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
	else if (m_pCurrentSongChannel == 0 && !m_nextSongPath.empty())
	{
		PlaySong(m_nextSongPath, m_musicVol);
		m_nextSongPath.clear();
	}
	ErrorCheck(m_pSystem->update());
}

void AudioManager::PlaySFX(const std::string & name, float volume, bool loop, bool is3D, Vector3D SourcePos,float minDis,float maxDist)
{
	volume = ScaledVol(volume);
	// Try to find sound effect and return if not found 
	FMOD::Sound* sound = TETRA_RESOURCES.GetSFX(TETRA_GAME_CONFIG.SFXDir() + name, SFX);
	if (!sound)
		return;

	//check if already playing     
	ChannelMap::iterator chMap = m_Channel[SFX].find(name);
	FMOD::Channel* channel = NULL;
	ErrorCheck(m_pSystem->playSound(sound, NULL, true, &channel));
	//save audiopath and its corresponding channel
	m_Channel[SFX].insert(std::make_pair(name, channel));

	if (loop)
		ErrorCheck(channel->setMode(FMOD_DEFAULT | FMOD_LOOP_NORMAL));
	else if (is3D)
	{
	
		ErrorCheck(channel->setMode(FMOD_DEFAULT | FMOD_3D| FMOD_3D_LINEARSQUAREROLLOFF));
		FMOD_VECTOR position = VectorToFmod(SourcePos);
		ErrorCheck(channel->set3DAttributes(&position, NULL));
		//channel->set3DMinMaxDistance(20.0f, 900.0f);
		channel->set3DMinMaxDistance(minDis, maxDist);
	}
	//set to the channel grp it belongs
	ErrorCheck(channel->setChannelGroup(m_pGroups[SFX]));
	ErrorCheck(channel->setVolume(volume));
	ErrorCheck(channel->setPaused(false));
}

void AudioManager::PlaySFX(const std::string & name, float volume)
{
	std::string soundFile = TETRA_GAME_CONFIG.SFXDir() + name;
	// Try to find sound effect and return if not found 
	FMOD::Sound* sound = TETRA_RESOURCES.GetSFX(soundFile, SFX);
	if (!sound)
		return;

	ChannelMap::iterator chMap = m_Channel[SFX].find(soundFile);
	//play sound with initial values

	FMOD::Channel* channel = NULL;
	ErrorCheck(m_pSystem->playSound(sound, NULL, true, &channel));
	//save audiopath and its corresponding channel
	m_Channel[SFX].insert(std::make_pair(soundFile, channel));
	//set to the channel grp it belongs
	ErrorCheck(channel->setChannelGroup(m_pGroups[SFX]));
	ErrorCheck(channel->setVolume(volume));
	ErrorCheck(channel->setPaused(false));

}

void AudioManager::PlaySong(const std::string & name, float volume)
{
	//store music volume
	m_musicVol = ScaledVol(volume);

	//ignore if song already playing
	std::string soundFile = TETRA_GAME_CONFIG.SFXDir() + name;
	if (m_currentSongPath == soundFile)
		return;

	//if song is playing stop them and set this as next song
	if (m_pCurrentSongChannel != 0)
	{
		StopSongs();
		m_nextSongPath = soundFile;
		return;
	}
	//find song in <-> sound map
	FMOD::Sound* sound = TETRA_RESOURCES.GetSFX(TETRA_GAME_CONFIG.SFXDir() + name, SONG);
	if (!sound)
		return;

	//start playing song with 0 vol and fade in
	m_currentSongPath = soundFile;
	ErrorCheck(m_pSystem->playSound(sound, NULL, true, &m_pCurrentSongChannel));
	ErrorCheck(m_pCurrentSongChannel->setChannelGroup(m_pGroups[SONG]));
	ErrorCheck(m_pCurrentSongChannel->setVolume(0.0f));
	ErrorCheck(m_pCurrentSongChannel->setPaused(false));
	m_fade = FADE_IN;
}

void AudioManager::StopAllSFXs()
{
	ErrorCheck(m_pGroups[SFX]->stop());
}

void AudioManager::StopSFX(std::string & name)
{
	std::string soundFile = TETRA_GAME_CONFIG.SFXDir() + name;
	FMOD::Sound* sound = TETRA_RESOURCES.GetSFX(soundFile, SFX);
	if (!sound)
		return;

	ChannelMap::iterator chMap = m_Channel[SFX].find(soundFile);
	chMap->second->stop();
}


void AudioManager::StopSongs()
{
	if (m_pCurrentSongChannel != 0)
		m_fade = FADE_OUT;
	m_nextSongPath.clear();
}

void AudioManager::MuteAllSFX()
{
	ErrorCheck(m_pGroups[SFX]->setMute(true));
}

void AudioManager::UnmuteAllSFX()
{
	ErrorCheck(m_pGroups[SFX]->setMute(false));
}

void AudioManager::SetMasterVolume(float volume)
{
	ErrorCheck(m_pMaster->setVolume(volume));
	//std::cout << "Master vol :" << volume << std::endl;
}

void AudioManager::SetSFXsVolume(float volume)
{
	ErrorCheck(m_pGroups[SFX]->setVolume(volume));
}

void AudioManager::SetSongsVolume(float volume)
{
	ErrorCheck(m_pGroups[SONG]->setVolume(volume));
	
}
float AudioManager::GetMasterVolume()
{
	float vol;
	ErrorCheck(m_pMaster->getVolume(&vol));
	return vol;
}
float AudioManager::GetSFXsVolume()
{
	float vol;
	ErrorCheck(m_pGroups[SFX]->getVolume(&vol));
	return vol;
}
float AudioManager::GetSongsVolume()
{
	float vol;
	//ErrorCheck(m_pCurrentSongChannel->getVolume(&vol));
	if (m_pCurrentSongChannel==nullptr)
	{
		std::cout << "song channel pointer is NULL\n";
	}
	m_pCurrentSongChannel->getVolume(&vol);
	return vol;
}
void AudioManager::TogglePause()
{
	for (int i = 0; i < CATEGORY_COUNT; i++)
	{
		ErrorCheck(m_pGroups[i]->getPaused(&m_isChannelGroupPaused));
		if (m_isChannelGroupPaused)
			ErrorCheck(m_pGroups[i]->setPaused(false));
		else
			ErrorCheck(m_pGroups[i]->setPaused(true));
	}
}
void AudioManager::PauseSound()
{
	m_pCurrentSongChannel->setPaused(true);
}
void AudioManager::PauseSFX(std::string & name)
{
	std::string soundFile = TETRA_GAME_CONFIG.SFXDir() + name;
	ChannelMap::iterator chMap = m_Channel[SFX].find(soundFile);
	if (chMap == m_Channel[SFX].end())
		return;

	chMap->second->setPaused(true);
}
void AudioManager::ResumeSound()
{
	m_pCurrentSongChannel->setPaused(false);
}
void AudioManager::ResumeSFX(std::string & name)
{
	std::string soundFile = TETRA_GAME_CONFIG.SFXDir() + name;
	ChannelMap::iterator chMap = m_Channel[SFX].find(soundFile);
	if (chMap == m_Channel[SFX].end())
		return;

	chMap->second->setPaused(false);
}
bool AudioManager::isSoundPlaying(std::string name)
{
	std::string soundFile = TETRA_GAME_CONFIG.SFXDir() + name;
	ChannelMap::iterator chMap = m_Channel[SFX].find(soundFile);
	if (chMap != m_Channel[SFX].end())
	{
		chMap->second->isPlaying(&m_isPlaying);
		return m_isPlaying;
	}
	else
		return false;
}

void AudioManager::ToggleMuteSounds()
{
	for (int i = 0; i < CATEGORY_COUNT; i++)
	{
		ErrorCheck(m_pGroups[i]->getMute(&m_isChannelGroupMute));
		if (m_isChannelGroupMute)
			ErrorCheck(m_pGroups[i]->setMute(false));
		else
			ErrorCheck(m_pGroups[i]->setMute(true));
	}
}

void AudioManager::MuteMusic()
{
	m_pCurrentSongChannel->setMute(true);
}

void AudioManager::ResumeMusic()
{
	m_pCurrentSongChannel->setMute(false);
}

void AudioManager::ErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK) {
		std::cout << FMOD_ErrorString(result) << std::endl;
	}
	return;
}
float vol;
void AudioManager::HandleEvent(Event* pEvent) {
	FloatData * pFloatData = pEvent->Data<FloatData>();
	InputButtonData * pButtonData = pEvent->Data<InputButtonData>();
	
	switch (pEvent->Type()) {
	case EVENT_INPUT_PAUSEMUSIC:
		
		if (pButtonData->m_isTrigger) TogglePause();
		break;

	case EVENT_ChangeBGMVol:
		
		if (!m_pCurrentSongChannel)
		{
			m_musicVol = pFloatData->mValue;
			//std::cout << "BGM volume : " << m_musicVol << std::endl;
		}
		else
		{
			SetSongsVolume(pFloatData->mValue);
			m_pCurrentSongChannel->getVolume(&vol);
			//std::cout << "BGM volume : " << vol << std::endl;
		}
		break;

	case EVENT_ChangeMasterVol:
		//FloatData * pFloatData = pEvent->Data<FloatData>();
		SetMasterVolume(pFloatData->mValue);
		m_pMaster->getVolume(&vol);
		//std::cout << "Master volume : " << vol << std::endl;
		break;

	case EVENT_ChangeSFXVol:
		//FloatData * pFloatData = pEvent->Data<FloatData>();
		SetSFXsVolume(pFloatData->mValue);
		m_pGroups[SFX]->getVolume(&vol);
		//std::cout << "SFX volume : " << vol << std::endl;
		break;
	}
}



FMOD_VECTOR AudioManager::VectorToFmod(const Vector3D& SourcePos) {
	FMOD_VECTOR fVec;
	fVec.x = SourcePos.x;
	fVec.y = SourcePos.y;
	fVec.z = SourcePos.z;
	return fVec;
}

void AudioManager::Set3dListener(const Vector3D & SourcePos/*, const Vector3D& vel*/)
{
	FMOD_VECTOR position = VectorToFmod(SourcePos);
	//FMOD_VECTOR velocity = VectorToFmod(vel);
	m_pSystem->set3DListenerAttributes(0, &position, 0 /*&velocity*/, 0, 0);
}

void AudioManager::SetFadeTime(float time)
{
	m_fadeTime = time;
}

float AudioManager::ScaledVol(float vol)
{
	return vol / 10.0f;
}




