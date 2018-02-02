#pragma once
#ifndef AUDIO_H
#define AUDIO_H

#include "Component.h"
#include "AudioManager.h"
class Audio:public Component
{
public:
	Audio();
	~Audio();

	// Inherited via Component
	virtual void Update(float dt) override;
	virtual void Serialize(json j) override;
	//void Load();

private:
	std::list<std::string> m_audioClips;
};

#endif

