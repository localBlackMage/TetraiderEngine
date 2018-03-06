//#include "SizeToScreen.h"
//#include "Transform.h"
//#include "Camera.h"
//#include "TetraiderAPI.h"
//#include "Math/MathLibs.h"

#include <Stdafx.h>

SizeToScreen::SizeToScreen() :
	Component(ComponentType::C_SizeToScreen)
{}

SizeToScreen::~SizeToScreen() {}

void SizeToScreen::Deactivate() { 
	pGO = nullptr; 
	m_pTransform = nullptr;  
	m_pCamera = nullptr;
}

void SizeToScreen::Serialize(const json& j) {
}

void SizeToScreen::LateInitialize()
{
	if (!m_pTransform) {
		assert(pGO && "No Game Object found. SizeToScreen component failed to operate.");
		m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
		assert(m_pTransform && "No Transform component found. SizeToScreen component failed to operate.\n");
	}

	if (!m_pCamera) {
		GameObject* pPrimaryCamera = TETRA_GAME_OBJECTS.GetPrimaryCamera();
		assert(pPrimaryCamera && "No primary Camera found. SizeToScreen component failed to operate.");
		m_pCamera = pPrimaryCamera->GetComponent<Camera>(ComponentType::C_Camera);
		assert(m_pCamera && "No primary Camera component component found. SizeToScreen component failed to operate.");
	}
}

void SizeToScreen::Update(float dt) {}

void SizeToScreen::LateUpdate(float dt) {
	m_pTransform->SetScale(m_pCamera->ViewWidth(), m_pCamera->ViewHeight());
}

void SizeToScreen::HandleEvent(Event* pEvent) {
}