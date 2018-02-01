#include "Agent.h"
#include "Transform.h"
#include "Body.h"
#include "PhysicsManager.h"
#include "../Source/Math/MathFunctions.h"

#include <iostream>

void Agent::Update(float dt) {
	m_currentVelocity = Lerp(m_currentVelocity, m_targetVelocity, dt*m_acceleration);
	m_pBody->SetVelocity(m_currentVelocity);
}

void Agent::Serialize(json j) {
	m_speed = ParseFloat(j, "speed");
	m_acceleration = ParseFloat(j, "acceleration");
}

void Agent::HandleEvent(Event* pEvent) {
	if (pEvent->Type() == EventType::EVENT_OnCollide) {
		OnCollide* pOnCollide = static_cast<OnCollide*>(pEvent);
		m_pTransform->SetPosition(m_pTransform->GetPosition() + pOnCollide->mtv.normal*pOnCollide->mtv.penetration);
	}
}

void Agent::LateInitialize() {
	if (!m_pTransform) {
		if (pGO)
			m_pTransform = static_cast<Transform*>(pGO->GetComponent(ComponentType::Transform));
		else {
			printf("No Game Object found. Controller component failed to operate.\n");
			return;
		}

		if (!m_pTransform) {
			printf("No Transform component found. Controller component failed to operate.\n");
			return;
		}
	}

	if (!m_pBody) {
		if (pGO)
			m_pBody = static_cast<Body*>(pGO->GetComponent(ComponentType::Body));
		else {
			printf("No Game Object found. Controller component failed to operate.\n");
			return;
		}

		if (!m_pBody) {
			printf("No Body component found. Controller component failed to operate.\n");
			return;
		}
	}
}