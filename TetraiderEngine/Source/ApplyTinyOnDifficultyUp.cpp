/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

ApplyTintOnDifficultyUp::ApplyTintOnDifficultyUp() :Component(ComponentType::C_ApplyTintOnDifficultyUp) {}
ApplyTintOnDifficultyUp::~ApplyTintOnDifficultyUp() {}


void ApplyTintOnDifficultyUp::Update(float dt)
{
}

void ApplyTintOnDifficultyUp::Serialize(const json & j)
{
	m_tintRate = ParseFloat(j, "tintRate");
}

void ApplyTintOnDifficultyUp::LateInitialize() {
	Sprite* pSprite = pGO->GetComponent<Sprite>(C_Sprite);
	if (pSprite) {
		pSprite->SetTintColor(Vector3D(1, 1 - m_tintRate*TETRA_LEVELS.GetLevelsCompleted(), 1 - m_tintRate*TETRA_LEVELS.GetLevelsCompleted(), 1));
	}
}

void ApplyTintOnDifficultyUp::HandleEvent(Event * pEvent)
{	
}
