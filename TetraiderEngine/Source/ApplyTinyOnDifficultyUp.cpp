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
