#include "GameObject.h"
#include "SnapToCamera.h"
#include "Transform.h"
#include "TetraiderAPI.h"
#include "Math/MathLibs.h"

SnapToCamera::SnapToCamera() :
	Component(ComponentType::C_SnapToCamera)
{}

SnapToCamera::~SnapToCamera() {}

void SnapToCamera::Deactivate() { 
	pGO = nullptr; 
	m_pTransform = nullptr;  
	m_pCameraTransform = nullptr;
}

void SnapToCamera::Serialize(const json& j) {
}

void SnapToCamera::LateInitialize()
{
	if (!m_pTransform) {
		assert(pGO && "No Game Object found. SnapToCamera component failed to operate.");
		m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
		assert(m_pTransform && "No Transform component found. SnapToCamera component failed to operate.");
	}

	if (!m_pCameraTransform) {
		GameObject* pPrimaryCamera = TETRA_GAME_OBJECTS.GetPrimaryCamera();
		assert(pPrimaryCamera && "No primary Camera found. SnapToCamera component failed to operate.");
		m_pCameraTransform = pPrimaryCamera->GetComponent<Transform>(ComponentType::C_Transform);
		assert(m_pCameraTransform && "No primary Camera Transform component found. SnapToCamera component failed to operate.");
	}
}

void SnapToCamera::Update(float dt) {}

void SnapToCamera::LateUpdate(float dt) {
	Vector3D cameraPos = m_pCameraTransform->GetPosition();
	cameraPos.z = m_pTransform->GetPosition().z;
	m_pTransform->SetPosition(cameraPos);
}

void SnapToCamera::HandleEvent(Event* pEvent) {}