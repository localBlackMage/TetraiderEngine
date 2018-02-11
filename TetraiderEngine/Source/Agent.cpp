#include "GameObject.h"
#include "Agent.h"
#include "Body.h"
#include "Transform.h"
#include "TetraiderAPI.h"
#include "Animation.h"
#include "Camera.h"
#include <iostream>

Agent::Agent(ComponentType _type) : 
	Component(_type), 
	m_speed(0.0f), 
	m_acceleration(0.0f), 
	m_deceleration(0.0f), 
	m_lookDirection(Vector3D(1, 0, 0)),
	m_knockBackSpeed(0),
	m_faceDirection(FaceDirection::Right)
{};

void Agent::Update(float dt) {
	m_currentVelocity = Lerp(m_currentVelocity, m_targetVelocity, dt*m_acceleration);
	m_pBody->SetVelocity(m_currentVelocity);

	if (m_pAnimation) {
		if (m_currentVelocity.SquareLength() < 100.0f) {
			m_pAnimation->Play(1);
		}
		else {
			m_pAnimation->Play(0);
		}
	}

	float angle = m_lookDirection.AngleDegrees();
	if (m_pTransform->GetScaleX() < 0) {
		if (angle > -90 && angle < 90) {
			pGO->HandleEvent(&Event(EVENT_FlipScaleX));
			m_faceDirection = FaceDirection::Right;
		}
	}
	else if (angle < -90 || angle > 90) {
		pGO->HandleEvent(&Event(EVENT_FlipScaleX));
		m_faceDirection = FaceDirection::Left;
	}
}

void Agent::Serialize(const json& j) {
	m_speed = ParseFloat(j, "speed");
	m_acceleration = ParseFloat(j, "acceleration");
	m_knockBackSpeed = ParseFloat(j, "knockBackSpeed");
}

void Agent::HandleEvent(Event* pEvent) {
	if (pEvent->Type() == EventType::EVENT_OnCollide) {
		OnCollideData* collisionData = pEvent->Data<OnCollideData>();
		if(collisionData->pGO->m_tag == T_Obstacle)
			m_pTransform->SetPosition(m_pTransform->GetPosition() + collisionData->mtv.normal*collisionData->mtv.penetration);
		else if(collisionData->pGO->m_tag == T_Enemy || collisionData->pGO->m_tag == T_Player)
			m_pTransform->SetPosition(m_pTransform->GetPosition() + collisionData->mtv.normal*collisionData->mtv.penetration*0.5f);
	}
	else if (pEvent->Type() == EventType::EVENT_OnTakeDamage) {
		HealthChangeData* healthData = pEvent->Data<HealthChangeData>();
		Vector3D dirOfAttack = m_pTransform->GetPosition() - healthData->m_sourceOfAttack;
		dirOfAttack.Normalize();
		AddVelocity(dirOfAttack*m_knockBackSpeed);
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

	if (!m_pAnimation) {
		if (pGO)
			m_pAnimation = pGO->GetComponent<Animation>(ComponentType::C_Animation);
		else {
			//printf("No Game Object found. Controller component failed to operate.\n");
			return;
		}

		if (!m_pAnimation) {
			//printf("No Body component found. Controller component failed to operate.\n");
			return;
		}
	}
}

Vector3D Agent::GetDirectionToMouse() {
	Vector3D mousePos = Vector3D((float)TETRA_INPUT.MousePosX(), (float)TETRA_INPUT.MousePosY(), 0);
	GameObject* mainCam = TETRA_GAME_OBJECTS.GetCamera(1);
	Camera* camComponent = mainCam->GetComponent<Camera>(ComponentType::C_Camera);
	Vector3D screenSpace = camComponent->TransformPointToScreenSpace(m_pTransform->GetPosition());
	Vector3D dirToMousePos = mousePos - screenSpace;
	dirToMousePos.y *= -1;
	dirToMousePos.Normalize();
	return dirToMousePos;
}