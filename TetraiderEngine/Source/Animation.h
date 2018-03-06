#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H

//#include "Component.h"
//
//class Sprite;

class Animation :public Component {
public:
	Animation();
	~Animation();
	static Component* CreateInstance() { return new Animation(); }
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt) {}
	virtual void Deactivate();
	bool IsPlaying() { return m_isPlaying; }

	void Play(int animation);
	void Play(int animation, bool isReverse);
	void Play(int animation, bool isReverse, float animationSpeedMultiplier);
private:
	void ChangeAnimation(int animation);

	float m_uStartPos;
	float m_vStartPos;
	int m_numberOfAnimations;
	int m_currentAnimation;
	int m_maxFrames;
	int m_reverse;

	json myAnimations;
	bool m_isLooping;
	float m_animationSpeed;
	int m_frames;
	int m_currentFrame;
	float m_elapsedTime;
	float m_speedMultiplier;
	bool m_isPlaying;
	bool m_isPlayOnAwake;
	bool m_isDisableRenderOnAnimComplete;
	Sprite* m_pSprite;
};

#endif

