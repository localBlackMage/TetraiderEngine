/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

StraightLineMovement::StraightLineMovement(): Component(ComponentType::C_StraightLineMovement) {}
StraightLineMovement::~StraightLineMovement() {}

void StraightLineMovement::Deactivate() {
	pGO = nullptr; 
	m_pTransform = nullptr;
}

void StraightLineMovement::Update(float dt) { 
	m_pTransform->Move(m_moveDirection * m_speed * dt);
}

void StraightLineMovement::Serialize(const json& j) {
	m_speed = ParseFloat(j, "speed");
	m_moveDirection = ParseVector3D(j, "direction");
}

void StraightLineMovement::LateInitialize() {
	if (!m_pTransform) {
		if (pGO)
			m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
		else {
			printf("No Game Object found. StraightLineMovement component failed to operate.\n");
			return;
		}

		if (!m_pTransform) {
			printf("No Transform component found.  StraightLineMovement component failed to operate.\n");
			assert(m_pTransform);
			return;
		}
	}
}

void StraightLineMovement::HandleEvent(Event* pEvent) {
}