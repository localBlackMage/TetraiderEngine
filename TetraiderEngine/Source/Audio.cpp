#include "Audio.h"
#include "TetraiderAPI.h"

Audio::Audio():Component(ComponentType::C_Audio) {}
Audio::~Audio() {}
void Audio::Update(float dt) {}

void Audio::Serialize(const json& j)
{
	m_audioClip = ParseString(j, "audioClip");
	m_volume = ParseFloat(j, "volume");
	m_Ismute = ParseBool(j, "isMute");
	m_isLoop = ParseBool(j, "isLoop");
	m_isPlayOnAwake = ParseBool(j, "isPlayOnAwake");
	m_isBGM = ParseBool(j, "isBGM");

	if(m_isBGM)
		TETRA_RESOURCES.LoadSong(m_audioClip);
	else
		TETRA_RESOURCES.LoadSFX(m_audioClip);
}

bool Audio::IsPlaying()
{
	return TETRA_AUDIO.isSoundPlaying(m_audioClip);
}

void Audio::Play()
{
	if(m_Ismute)
		TETRA_AUDIO.PlaySFX(m_audioClip, 0); // TODO: Add loop to function parameters
	else
		TETRA_AUDIO.PlaySFX(m_audioClip, m_volume); // TODO: Add loop to function parameters
}

void Audio::LateInitialize()
{
	if (m_isPlayOnAwake)
	{
		if(m_isBGM)
			TETRA_AUDIO.PlaySong(m_audioClip); //TODO: Allow volume control here
		else
			TETRA_AUDIO.PlaySFX(m_audioClip, m_volume); // TODO: Add loop to function parameters
	}
}

void Audio::Stop() {
	// Stops audio from playing completely (resets)
}

void Audio::Pause() {
	// Stop aduio at current time
}

void Audio::Resume() {
	// Resumes audio from where if stopped
}