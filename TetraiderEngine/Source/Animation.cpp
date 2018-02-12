#include "GameObject.h"
#include "Animation.h"
#include "Sprite.h"
#include "TetraiderAPI.h"
#include <iostream>

using  namespace JsonReader;

Animation::Animation(): 
	Component(ComponentType::C_Animation), 
	m_animationSpeed(0), 
	m_uStartPos(1), 
	m_vStartPos(1), 
	m_elapsedTime(-1),
	m_reverse(1), 
	m_speedMultiplier(1),
	m_isPlaying(false),
	m_isLooping(false),
	m_isPlayOnAwake(false),
	m_isDisableRenderOnAnimComplete(false)
{}

Animation::~Animation() {}

void Animation::Update(float dt) {
	if (m_elapsedTime > 1) {
		++m_currentFrame;
		if (m_currentFrame >= m_frames) {
			if (!m_isLooping) {
				if(m_isDisableRenderOnAnimComplete)
					pGO->m_isRender = false;

				m_isPlaying = false;
			}

			m_currentFrame = 0;
		}

		m_elapsedTime = 0;
	}

	m_pSprite->SetTileX(m_uStartPos);
	m_pSprite->SetTileY(m_vStartPos);
	m_pSprite->SetUOffset(m_uStartPos*m_currentFrame*m_reverse);
	m_pSprite->SetVOffset(m_vStartPos*m_currentAnimation);

	if (!m_isPlaying) 
		return;

	m_elapsedTime += dt*m_animationSpeed*m_speedMultiplier;
}

void Animation::Play(int animation) {
	if (animation != m_currentAnimation || !m_isPlaying)
		ChangeAnimation(animation);

	m_isPlaying = true;
}

void Animation::Play(int animation, bool isReverse) {
	if (isReverse)
		m_reverse = -1;
	else
		m_reverse = 1;

	Play(animation);
}

void Animation::Play(int animation, bool isReverse, float animationSpeedMultiplier) {
	m_speedMultiplier = animationSpeedMultiplier;

	Play(animation, isReverse);
}

void Animation::ChangeAnimation(int animation) {
	if (animation >= m_numberOfAnimations) {
		printf("Attempting to play animation out of animation component index. Operation failed\n");
		return;
	}

	pGO->m_isRender = true;
	m_elapsedTime = 0;
	m_currentFrame = 0;
	m_speedMultiplier = 1;
	m_currentAnimation = animation;
	m_reverse = 1;
	m_frames = ParseInt(myAnimations[m_currentAnimation],"frames");
	m_animationSpeed = ParseFloat(myAnimations[m_currentAnimation], "animationSpeed");
	m_isLooping = ParseBool(myAnimations[m_currentAnimation], "isLooping");
}

void Animation::Serialize(const json& j) {
	m_maxFrames = 0;
	m_currentAnimation = ParseInt(j, "defaultAnimation");
	m_isDisableRenderOnAnimComplete = ParseBool(j, "isDisableRenderOnAnimComplete");
	myAnimations = j["MY_ANIMATIONS"];
	m_frames = ParseInt(myAnimations[m_currentAnimation],"frames");
	m_animationSpeed = myAnimations[m_currentAnimation]["animationSpeed"];
	m_isLooping = ParseBool(myAnimations[m_currentAnimation], "isLooping");
	m_numberOfAnimations = myAnimations.size();
	m_isPlayOnAwake = ParseBool(myAnimations[m_currentAnimation], "isPlayOnAwake");

	for (int i = 0; i < m_numberOfAnimations; ++i) {
		int frames = ParseInt(myAnimations[i], "frames");
		if (frames > m_maxFrames)
			m_maxFrames = frames;
	}

	m_uStartPos = 1 / (float)m_maxFrames;
	m_vStartPos = 1 / (float)m_numberOfAnimations;

	if (m_isPlayOnAwake) {
		Play(m_currentAnimation);
	}
}

void Animation::LateInitialize() {
	if (!m_pSprite) {
		if (pGO)
			m_pSprite = pGO->GetComponent<Sprite>(ComponentType::C_Sprite);
		else {
			printf("No Game Object found. Animation component failed to operate.\n");
			return;
		}

		if (!m_pSprite) {
			printf("No Sprite component found. Animation component failed to operate.\n");
			assert(m_pSprite);
			return;
		}
	}

	m_pSprite->SetTileX(m_uStartPos);
	m_pSprite->SetTileY(m_vStartPos);
	m_pSprite->SetUOffset(m_uStartPos*m_currentFrame*m_reverse);
	m_pSprite->SetVOffset(m_vStartPos*m_currentAnimation);

	if (!m_isPlayOnAwake && m_isDisableRenderOnAnimComplete) {
		pGO->m_isRender = false;
	}
}