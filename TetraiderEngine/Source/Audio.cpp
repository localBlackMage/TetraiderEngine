#include <Stdafx.h>

Audio::Audio() :Component(ComponentType::C_Audio), m_currentIndex(0) {}
Audio::~Audio() 
{
	if (m_isBGM)
	{
		TETRA_AUDIO.StopSongs();
	}
}

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
	if (j["audioClip"].is_array())
		m_audioClip = (ParseStringList(j, "audioClip"));
	else
		m_audioClip.push_back(ParseString(j, "audioClip"));

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
		TETRA_RESOURCES.LoadSong(m_audioClip[m_currentIndex]);
		TETRA_AUDIO.SetFadeTime(m_fadeTime);
	}	
	else {
		for (int i = 0; i < m_audioClip.size(); ++i) {
			TETRA_RESOURCES.LoadSFX(m_audioClip[i]);
		}
	}
}

bool Audio::IsPlaying()
{
	return TETRA_AUDIO.isSoundPlaying(m_audioClip[m_currentIndex]);
}

void Audio::Play(int index)
{
	if (index >= m_audioClip.size())
		return;
	else {
		m_currentIndex = index;
	}

	 if (m_Ismute)
		TETRA_AUDIO.PlaySFX(m_audioClip[m_currentIndex], 0, m_isLoop, m_is3D, pGO->GetComponent<Transform>(ComponentType::C_Transform)->GetPosition(),m_minDist,m_maxDist);
	else
		TETRA_AUDIO.PlaySFX(m_audioClip[m_currentIndex], m_volume, m_isLoop, m_is3D, pGO->GetComponent<Transform>(ComponentType::C_Transform)->GetPosition(), m_minDist, m_maxDist);
}

void Audio::LateInitialize()
{
	if (m_isPlayOnAwake)
	{
		if (m_isBGM)
			TETRA_AUDIO.PlaySong(m_audioClip[m_currentIndex], m_volume);
		else
			TETRA_AUDIO.PlaySFX(m_audioClip[m_currentIndex], m_volume, m_isLoop, m_is3D, pGO->GetComponent<Transform>(ComponentType::C_Transform)->GetPosition(), m_minDist, m_maxDist);
	}
}

void Audio::Stop() 
{
	// Stops audio from playing completely (resets)
	if (m_isBGM)
		TETRA_AUDIO.StopSongs();
	else
		TETRA_AUDIO.StopSFX(m_audioClip[m_currentIndex]);
}

void Audio::Pause() {
	// Stop aduio at current time
	if (m_isBGM)
		TETRA_AUDIO.PauseSound();
	else
		TETRA_AUDIO.PauseSFX(m_audioClip[m_currentIndex]);
}

void Audio::Resume()
{
	if (m_isBGM)
		TETRA_AUDIO.ResumeSound();
	else
		TETRA_AUDIO.ResumeSFX(m_audioClip[m_currentIndex]);
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

