#include "GameObject.h"
#include "CamFollow.h"
#include "Transform.h"
#include "TetraiderAPI.h"
#include "Math/MathLibs.h"

CamFollow::CamFollow() :
	Component(ComponentType::C_CamFollow),
	m_followSpeed(0.0f)
{}

CamFollow::~CamFollow() {}

void CamFollow::DeActivate() { 
	pGO = nullptr; 
	m_pTransform = nullptr;  
	m_pTarget = nullptr; 
}

void CamFollow::Serialize(const json& j) {
	m_followSpeed = ParseFloat(j, "followSpeed");
	m_defaultTargetTag = ParseString(j, "defaultTargetTag");
}

void CamFollow::LateInitialize()
{
	if (!m_pTransform) {
		if (pGO)
			m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
		else {
			printf("No Game Object found. Camfollow component failed to operate.\n");
			return;
		}

		if (!m_pTransform) {
			printf("No Transform component found. Camfollow component failed to operate.\n");
			assert(m_pTransform);
			return;
		}
	}
}

void CamFollow::Update(float dt) {}

void CamFollow::LateUpdate(float dt) {
	Transform* targetTransform = m_pTarget->GetComponent<Transform>(ComponentType::C_Transform);
	float z = m_pTransform->GetPosition().z;
	Vector3D lerpPosition = Lerp(m_pTransform->GetPosition(), targetTransform->GetPosition(), m_followSpeed*dt);
	m_pTransform->SetPosition(Vector3D(lerpPosition.x, lerpPosition.y, z));
}

void CamFollow::HandleEvent(Event* pEvent) {
	if (pEvent->Type() == EVENT_OnLevelInitialized) {
		m_pTarget = TETRA_GAME_OBJECTS.FindObjectWithTag(TETRA_GAME_OBJECTS.FindTagWithString(m_defaultTargetTag));
	}
}