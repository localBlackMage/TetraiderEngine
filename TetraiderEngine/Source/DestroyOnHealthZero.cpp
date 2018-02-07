#include "GameObject.h"
#include "DestroyOnHealthZero.h"
#include "TetraiderAPI.h"

DestroyOnHealthZero::DestroyOnHealthZero(): Component(ComponentType::C_DestroyOnHealthZero), m_destroyIn(0), m_timeStamp(0), m_isDestory(false) {}
DestroyOnHealthZero::~DestroyOnHealthZero() {}

void DestroyOnHealthZero::Update(float dt) {
	if (m_isDestory) {
		if (TETRA_FRAMERATE.GetElapsedTime() - m_timeStamp >= m_destroyIn) {
			pGO->Destroy();
		}
	}
}

void DestroyOnHealthZero::Serialize(const json& j) {
	m_destroyIn = ParseFloat(j, "destroyIn");
}

void DestroyOnHealthZero::LateInitialize() {}

void DestroyOnHealthZero::HandleEvent(Event* pEvent) {
	if (pEvent->Type() == EventType::EVENT_OnHealthZero) {
		m_timeStamp = TETRA_FRAMERATE.GetElapsedTime() - 0.001f;
		m_isDestory = true;
	}
}