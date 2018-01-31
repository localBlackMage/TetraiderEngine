#include "CamFollow.h"
#include "Transform.h"
#include "Math/MathFunctions.h"

CamFollow::CamFollow() :
	Component(ComponentType::CamFollow),
	m_followSpeed(0.0f)
{}

CamFollow::~CamFollow() {}

void CamFollow::Serialize(json j) {
	m_followSpeed = ParseFloat(j, "followSpeed");
	m_defaultTargetTag = ParseString(j, "defaultTargetTag");
}

void CamFollow::LateInitialize()
{
	if (!m_pTransform) {
		if (pGO)
			m_pTransform = static_cast<Transform*>(pGO->GetComponent(ComponentType::Transform));
		else {
			printf("No Game Object found. Camfollow component failed to operate.\n");
			return;
		}

		if (!m_pTransform) {
			printf("No Transform component found. Camfollow component failed to operate.\n");
			return;
		}
	}
}

void CamFollow::Update(float dt) {}

void CamFollow::LateUpdate(float dt) {
	Transform* targetTransform = static_cast<Transform*>(m_pTarget->GetComponent(ComponentType::Transform));
	float z = m_pTransform->GetPosition().z;
	Vector3D lerpPosition = Lerp(m_pTransform->GetPosition(), targetTransform->GetPosition(), m_followSpeed*dt);
	m_pTransform->SetPosition(Vector3D(lerpPosition.x, lerpPosition.y, z));
}

void CamFollow::HandleEvent(Event* pEvent) {
	if (pEvent->Type() == EVENT_OnLevelInitialized) {
		m_pTarget = T_GAME_OBJECTS.FindObjectWithTag(T_GAME_OBJECTS.FindTagWithString(m_defaultTargetTag));
	}
}