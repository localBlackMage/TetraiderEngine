#include "GameObject.h"
#include "Agent.h"
#include "Body.h"
#include "Transform.h"
#include "TetraiderAPI.h"
#include <iostream>

Agent::Agent(ComponentType _type) : 
	Component(_type), 
	m_speed(0.0f), 
	m_acceleration(0.0f), 
	m_deceleration(0.0f), 
	m_lookDirection(Vector3D(1, 0, 0))
{};

void Agent::Update(float dt) {
	m_currentVelocity = Lerp(m_currentVelocity, m_targetVelocity, dt*m_acceleration);
	m_pBody->SetVelocity(m_currentVelocity);
}

void Agent::Serialize(const json& j) {
	m_speed = ParseFloat(j, "speed");
	m_acceleration = ParseFloat(j, "acceleration");
}

void Agent::HandleEvent(Event* pEvent) {
	if (pEvent->Type() == EventType::EVENT_OnCollide) {
		OnCollideData* collisionData = pEvent->Data<OnCollideData>();
		if (collisionData->pGO->m_tag == T_Projectile)
			return;

		m_pTransform->SetPosition(m_pTransform->GetPosition() + collisionData->mtv.normal*collisionData->mtv.penetration);
	}
}

void Agent::LateInitialize() {
	if (!m_pTransform) {
		if (pGO)
			m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
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
			m_pBody = pGO->GetComponent<Body>(ComponentType::C_Body);
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