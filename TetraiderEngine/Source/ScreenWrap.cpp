#include "ScreenWrap.h"
#include "Transform.h"
#include "TetraiderAPI.h"
#include "Math/MathLibs.h"

ScreenWrap::ScreenWrap() :
	Component(ComponentType::C_ScreenWrap),
	m_xAllowance(0.f), m_yAllowance(0.f)
{}

ScreenWrap::~ScreenWrap() {}

void ScreenWrap::Deactivate() { 
	pGO = nullptr; 
	m_pTransform = nullptr;  
	m_pCameraTransform = nullptr;
}

void ScreenWrap::Serialize(const json& j) {
	m_xAllowance = ValueExists(j, "xAllowance")  ? j["xAllowance"] : -1.f;
	m_yAllowance = ValueExists(j, "yAllowance") ? j["yAllowance"] : -1.f;
}

void ScreenWrap::LateInitialize()
{
	if (!m_pTransform) {
		if (pGO)
			m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
		else {
			printf("No Game Object found. ScreenWrap component failed to operate.\n");
			return;
		}

		if (!m_pTransform) {
			printf("No Transform component found. ScreenWrap component failed to operate.\n");
			assert(m_pTransform);
			return;
		}
	}

	if (!m_pCameraTransform) {
		if (pGO)
			m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
		else {
			printf("No Game Object found. ScreenWrap component failed to operate.\n");
			return;
		}

		if (!m_pCameraTransform) {
			printf("No Transform component found. ScreenWrap component failed to operate.\n");
			assert(m_pTransform);
			return;
		}
	}
}

void ScreenWrap::Update(float dt) {}

void ScreenWrap::LateUpdate(float dt) {
	//Transform* targetTransform = m_pTarget->GetComponent<Transform>(ComponentType::C_Transform);
	//float z = m_pTransform->GetPosition().z;
	//Vector3D lerpPosition = Lerp(m_pTransform->GetPosition(), targetTransform->GetPosition(), m_followSpeed*dt);
	//m_pTransform->SetPosition(Vector3D(lerpPosition.x, lerpPosition.y, z));
}

void ScreenWrap::HandleEvent(Event* pEvent) {
	//if (pEvent->Type() == EVENT_OnLevelInitialized) {
	//	m_pTarget = TETRA_GAME_OBJECTS.FindObjectWithTag(TETRA_GAME_OBJECTS.FindTagWithString(m_defaultTargetTag));
	//	Transform* targetTransform = m_pTarget->GetComponent<Transform>(ComponentType::C_Transform);
	//	float z = m_pTransform->GetPosition().z;
	//	m_pTransform->SetPosition(Vector3D(targetTransform->GetPosition().x, targetTransform->GetPosition().y, z));
	//}
}