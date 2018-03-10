//#include "Audio.h"
//#include "TetraiderAPI.h"
//#include "Transform.h"

#include <Stdafx.h>

Audio::Audio() :Component(ComponentType::C_Audio) {}
Audio::~Audio() {}

void Audio::Deactivate() {
	pGO = nullptr;
}

void Audio::Update(float dt) 
{
	if (m_Ismute && m_isBGM)
	{
		TETRA_AUDIO.MuteMusic();
	}
}

void Audio::Serialize(const json& j)
{
	m_audioClip = ParseString(j, "audioClip");
	m_volume = ParseFloat(j, "volume");
	m_Ismute = ParseBool(j, "isMute");
	m_isLoop = ParseBool(j, "isLoop");
	m_isPlayOnAwake = ParseBool(j, "isPlayOnAwake");
	m_isBGM = ParseBool(j, "isBGM");
	m_is3D = ParseBool(j, "is3D");
	m_minDist = ParseFloat(j,"minDistance");
	m_maxDist = ParseFloat(j, "maxDistance");
	m_fadeTime= ParseFloat(j, "fadeTime");

	if (m_isBGM)
	{
		TETRA_RESOURCES.LoadSong(m_audioClip);
		TETRA_AUDIO.SetFadeTime(m_fadeTime);
	}	
	else
		TETRA_RESOURCES.LoadSFX(m_audioClip);
}

bool Audio::IsPlaying()
{
	return TETRA_AUDIO.isSoundPlaying(m_audioClip);
}

void Audio::Play()
{
	//temp fix
	if (m_isBGM)
	{
		TETRA_AUDIO.PlaySong(m_audioClip, m_volume);	
	}
	else if (m_Ismute)
		TETRA_AUDIO.PlaySFX(m_audioClip, 0, m_isLoop, m_is3D, pGO->GetComponent<Transform>(ComponentType::C_Transform)->GetPosition(),m_minDist,m_maxDist);
	else
		TETRA_AUDIO.PlaySFX(m_audioClip, m_volume, m_isLoop, m_is3D, pGO->GetComponent<Transform>(ComponentType::C_Transform)->GetPosition(), m_minDist, m_maxDist);
}

void Audio::LateInitialize()
{
	//return;
	//fix late initialize
	if (m_isPlayOnAwake)
	{
		if (m_isBGM)
			TETRA_AUDIO.PlaySong(m_audioClip, m_volume);
		else
			TETRA_AUDIO.PlaySFX(m_audioClip, m_volume, m_isLoop, m_is3D, pGO->GetComponent<Transform>(ComponentType::C_Transform)->GetPosition());
	}
}

void Audio::Stop() 
{
	// Stops audio from playing completely (resets)
	if (m_isBGM)
		TETRA_AUDIO.StopSongs();
	else
		TETRA_AUDIO.StopSFX(m_audioClip);
}

void Audio::Pause() {
	// Stop aduio at current time
	if (m_isBGM)
		TETRA_AUDIO.PauseSound();
	else
		TETRA_AUDIO.PauseSFX(m_audioClip);
}

void Audio::Resume()
{
	if (m_isBGM)
		TETRA_AUDIO.ResumeSound();
	else
		TETRA_AUDIO.ResumeSFX(m_audioClip);
}

void Audio::ToggleMuteAll()
{
	TETRA_AUDIO.ToggleMuteSounds();
}

void Audio::Mute()
{
	if (m_isBGM)
		TETRA_AUDIO.MuteMusic();
}

void Audio::UnMute()
{
	if (m_isBGM)
		TETRA_AUDIO.ResumeMusic();
}

