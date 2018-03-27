#include <Stdafx.h>

FollowCamera::FollowCamera() :
	Component(ComponentType::C_FollowCamera)
{}

FollowCamera::~FollowCamera() {}

void FollowCamera::Deactivate() { 
	pGO = nullptr; 
	m_pTransform = nullptr;  
	m_pCameraTransform = nullptr;
}

void FollowCamera::Serialize(const json& j) {
}

void FollowCamera::LateInitialize()
{
	if (!m_pTransform) {
		assert(pGO && "No Game Object found. FollowCamera component failed to operate.");
		m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
		assert(m_pTransform && "No Transform component found. FollowCamera component failed to operate.");
	}

	if (!m_pCameraTransform) {
		GameObject* pPrimaryCamera = TETRA_GAME_OBJECTS.GetPrimaryCamera();
		assert(pPrimaryCamera && "No primary Camera found. FollowCamera component failed to operate.");
		m_pCameraTransform = pPrimaryCamera->GetComponent<Transform>(ComponentType::C_Transform);
		assert(m_pCameraTransform && "No primary Camera Transform component found. FollowCamera component failed to operate.");
	}
}

void FollowCamera::Update(float dt) {}

void FollowCamera::LateUpdate(float dt) {
	m_pTransform->Move(m_pCameraTransform->GetMovement());
}

void FollowCamera::HandleEvent(Event* pEvent) {}