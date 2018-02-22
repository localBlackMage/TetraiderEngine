#include "GameObject.h"
#include "WaveMovement.h"
#include "Math\MathFunctions.h"
#include "Transform.h"
#include "TetraiderAPI.h"

WaveMovement::WaveMovement(): Component(ComponentType::C_WaveMovement) {}
WaveMovement::~WaveMovement() {}

void WaveMovement::Deactivate() {
	pGO = nullptr; 
	m_pTransform = nullptr;
}

void WaveMovement::Update(float dt) { 
	float value = SinWave(m_amplitude, m_frequency, m_phaseShift, TETRA_FRAMERATE.GetElapsedTime());

	if (m_isAbsoluteValue)
		value = fabsf(value);

	if (m_isModifyXPos && !m_isModifyYPos) {
		Vector3D newPos(m_pInitialPos.x + value, m_pInitialPos.y, m_pInitialPos.z);
		m_pTransform->SetPosition(newPos);
	}
	else if (m_isModifyYPos && !m_isModifyXPos) {
		Vector3D newPos(m_pInitialPos.x, m_pInitialPos.y + value, m_pInitialPos.z);
		m_pTransform->SetPosition(newPos);
	}
	else {
		Vector3D newPos(m_pInitialPos.x + value, m_pInitialPos.y + value, m_pInitialPos.z);
		m_pTransform->SetPosition(newPos);
	}

}

void WaveMovement::Serialize(const json& j) {
	m_isModifyXPos = ParseBool(j, "isModifyXPos");
	m_isModifyYPos = ParseBool(j, "isModifyYPos");
	m_isAbsoluteValue = ParseBool(j, "isAbsoluteValue");
	m_frequency = ParseFloat(j, "frequency");
	m_phaseShift = ParseFloat(j, "phaseShift");
	m_amplitude = ParseFloat(j, "amplitude");
}

void WaveMovement::LateInitialize() {
	if (!m_pTransform) {
		if (pGO)
			m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
		else {
			printf("No Game Object found. Wave movement component failed to operate.\n");
			return;
		}

		if (!m_pTransform) {
			printf("No Transform component found.  Wave movement component failed to operate.\n");
			assert(m_pTransform);
			return;
		}
	}
}

void WaveMovement::HandleEvent(Event* pEvent) {
	if(pEvent->Type() == EVENT_OnLevelInitialized)
		m_pInitialPos = m_pTransform->GetPosition();
}