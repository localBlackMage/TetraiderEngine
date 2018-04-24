/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Sujay Shah>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef AUDIO_H
#define AUDIO_H

class Audio:public Component
{
public:
	Audio();
	~Audio();
	static Component* CreateInstance() { return new Audio(); }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt) {}
	virtual void HandleEvent(Event* pEvent) {}

	bool IsPlaying();
	void Play(int index = 0);
	void Stop();
	void Pause();
	void Resume();
	void SetVolume(float volume) { m_volume = volume; }
	void Loop(bool active) { m_isLoop = active; }
	void ToggleMuteAll(); /*{ m_Ismute = active; }*/
	void Mute();
	void UnMute();
	int getFadeState() { return fadeState; }
	void setFadeState(int fade) { fadeState = fade; }
private:
	std::vector<std::string> m_audioClip;
	int m_currentIndex;
	float m_volume;
	//min and max values for sound attenuation to have effect
	float m_minDist, m_maxDist;
	bool m_Ismute;
	bool m_isLoop;
	bool m_is3D;
	bool m_isPlayOnAwake;
	bool m_isBGM;
	float m_fadeTime;
	int fadeState;
	//0-none, 1-fade in, 2-fade out.
};

#endif

