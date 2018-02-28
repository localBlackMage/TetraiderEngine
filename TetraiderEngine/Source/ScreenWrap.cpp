#include "ScreenWrap.h"
#include "Transform.h"
#include "Camera.h"
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
		assert(pGO, "No Game Object found. ScreenWrap component failed to operate.");
		m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
		assert(m_pTransform, "No Transform component found. ScreenWrap component failed to operate.\n");
	}

	if (!m_pCameraTransform) {
		GameObject* pPrimaryCamera = TETRA_GAME_OBJECTS.GetPrimaryCamera();
		assert(pPrimaryCamera, "No primary Camera found. ScreenWrap component failed to operate.");
		m_pCameraTransform = pPrimaryCamera->GetComponent<Transform>(ComponentType::C_Transform);
		assert(m_pCameraTransform, "No primary Camera Transform component found. ScreenWrap component failed to operate.");
		m_pCamera = pPrimaryCamera->GetComponent<Camera>(ComponentType::C_Camera);
		assert(m_pCamera, "No primary Camera component component found. ScreenWrap component failed to operate.");
	}
}

void ScreenWrap::Update(float dt) {}

void ScreenWrap::LateUpdate(float dt) {	
	Vector3D pos = m_pTransform->GetPosition();
	Vector3D cameraPos = m_pCameraTransform->GetPosition();
	Vector3D toCamera = cameraPos - pos;

	if (m_xAllowance >= 0.f) {
		float maxX = m_pCamera->ViewWidth() / 2.f + m_xAllowance;
		// Left
		if (pos.x < cameraPos.x && maxX - toCamera.x <= 0.f) {
			pos.x = cameraPos.x + maxX - 10.f;
		}
		// Right
		else if (maxX - fabsf(toCamera.x) <= 0.f) {
			pos.x = cameraPos.x - maxX + 10.f;
		}
	}

	if (m_yAllowance >= 0.f) {
		float maxY = m_pCamera->ViewHeight() / 2.f + m_yAllowance;
		// Above
		if (pos.y < cameraPos.y && maxY - toCamera.y <= 0.f) {
			pos.y = cameraPos.y + maxY - 10.f;
		}
		// Below
		else if (maxY - fabsf(toCamera.y) <= 0.f) {
			pos.y = cameraPos.y - maxY + 10.f;
		}
	}
	
	m_pTransform->SetPosition(pos);
}

void ScreenWrap::HandleEvent(Event* pEvent) {
}