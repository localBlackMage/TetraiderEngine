/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

Spinner::Spinner() : Component(ComponentType::C_Spinner), m_speed(0.0f) {}
Spinner::~Spinner() {}

void Spinner::Update(float dt) {
	m_pTransform->RotateY(m_speed * dt);
}

void Spinner::LateUpdate(float dt) {}

void Spinner::Deactivate() {
	pGO = nullptr;
}

void Spinner::Serialize(const json& j) {
	m_speed = ParseFloat(j, "speed");
}

void Spinner::Override(const json& j) {}

void Spinner::LateInitialize() {
	if (!m_pTransform) {
		assert(pGO && "No Game Object found. Controller component failed to operate.");
		m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
		assert(m_pTransform && "No Transform component found. Controller component failed to operate.");
	}
}

void Spinner::HandleEvent(Event* pEvent) {
}
