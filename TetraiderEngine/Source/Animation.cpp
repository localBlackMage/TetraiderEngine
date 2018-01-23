#include "Animation.h"
#include "Sprite.h"
#include "InputManager.h"
#include <iostream>

using  namespace JsonReader;

Animation::Animation(): 
	Component(ComponentType::Animation), 
	m_animationSpeed(0), 
	m_uStartPos(1), 
	m_vStartPos(1), 
	m_elapsedTime(-1),
	m_reverse(1), 
	m_speedMultiplier(1) 
{}

Animation::~Animation() {}

void Animation::Update(float dt) {
	if (m_elapsedTime > 1) {
		++m_currentFrame;
		if (m_currentFrame >= m_frames && m_isLooping)
			m_currentFrame = 0;

		m_elapsedTime = 0;
	}

	if (m_currentFrame == m_frames && !m_isLooping)
		return;

	m_pSprite->SetTileX(m_uStartPos);
	m_pSprite->SetTileY(m_vStartPos);
	m_pSprite->SetUOffset(m_uStartPos*m_currentFrame*m_reverse);
	m_pSprite->SetVOffset(m_vStartPos*m_currentAnimation);

	m_elapsedTime += dt*m_animationSpeed*m_speedMultiplier;

	// Test code remove later
	/*InputManager& inputMngr = InputManager::GetInstance();

	if(inputMngr.IsKeyTriggered(SDL_SCANCODE_1)) {
		ChangeAnimation(0);
	}
	if (inputMngr.IsKeyTriggered(SDL_SCANCODE_2)) {
		ChangeAnimation(1);
	}*/
}

void Animation::Play(int animation) {
	if (animation != m_currentAnimation)
		ChangeAnimation(animation);
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

	m_elapsedTime = 0;
	m_currentFrame = 0;
	m_speedMultiplier = 1;
	m_currentAnimation = animation;
	m_reverse = 1;
	m_frames = ParseInt(myAnimations[m_currentAnimation],"frames");
	m_animationSpeed = ParseFloat(myAnimations[m_currentAnimation], "animationSpeed");
	m_isLooping = ParseBool(myAnimations[m_currentAnimation], "isLooping");
}

void Animation::Serialize(json j) {
	m_maxFrames = 0;
	m_currentAnimation = ParseInt(j, "defaultAnimation");
	myAnimations = j["MY_ANIMATIONS"];
	m_frames = ParseInt(myAnimations[m_currentAnimation],"frames");
	m_animationSpeed = myAnimations[m_currentAnimation]["animationSpeed"];
	m_isLooping = ParseBool(myAnimations[m_currentAnimation], "isLooping");
	m_numberOfAnimations = myAnimations.size();

	for (int i = 0; i < m_numberOfAnimations; ++i) {
		int frames = ParseInt(myAnimations[i], "frames");
		if (frames > m_maxFrames)
			m_maxFrames = frames;
	}

	m_uStartPos = 1 / (float)m_maxFrames;
	m_vStartPos = 1 / (float)m_numberOfAnimations;
}

void Animation::LateInitialize() {
	if (!m_pSprite) {
		if (pGO)
			m_pSprite = static_cast<Sprite*>(pGO->GetComponent(ComponentType::Sprite));
		else {
			printf("No Game Object found. Animation component failed to operate.\n");
			return;
		}

		if (!m_pSprite) {
			printf("No Sprite component found. Animation component failed to operate.\n");
			return;
		}
	}
}