/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>


Agent::Agent(ComponentType _type) :
	Component(_type),
	m_speed(0.0f),
	m_acceleration(0.0f),
	m_deceleration(0.0f),
	m_lookDirection(Vector3D(1, 0, 0)),
	m_knockBackMultiplier(1),
	m_faceDirection(FaceDirection::Right),
	m_isIgnoreHazards(false),
	m_isDead(false),
	m_isControlAnimationOnVelocity(true),
	m_isIgnoreAcid(false)
{};

void Agent::Deactivate() {
	pGO = nullptr; 
	m_pTransform = nullptr;
	m_pAnimation = nullptr; 
	m_pBody = nullptr; 
	m_isIgnoreHazards = false; 
}

void Agent::Update(float dt) {
	if (m_isDead)
		return;

	m_currentVelocity = Lerp(m_currentVelocity, m_targetVelocity, dt*m_acceleration);
	//m_pBody->SetVelocity(m_currentVelocity);

	if (m_pAnimation && m_isControlAnimationOnVelocity) {
		if (m_currentVelocity.SquareLength()  < 200.0f)
			m_pAnimation->Play(1);
		else if(m_currentVelocity.SquareLength())
			m_pAnimation->Play(0);
	}

	SetOrientation();
}

void Agent::SetOrientation() {
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
	m_knockBackMultiplier = ParseFloat(j, "knockBackMultiplier");
	m_isIgnoreHazards = ParseBool(j, "isIgnoreHazards");
	m_isIgnoreAcid = ParseBool(j, "isIgnoreAcid");
}

void Agent::HandleEvent(Event* pEvent) {
	if (pEvent->Type() == EventType::EVENT_OnCollide) {
		OnCollideData* collisionData = pEvent->Data<OnCollideData>();
		if (collisionData->pGO->m_tag == T_Hazard || collisionData->pGO->m_tag == T_Projectile)
			return;
		else if(collisionData->pGO->m_tag == T_Enemy || collisionData->pGO->m_tag == T_Player)
			m_pTransform->SetPosition(m_pTransform->GetPosition() + collisionData->mtv.normal*collisionData->mtv.penetration*0.5f);
		else if(collisionData->pGO->m_tag == T_Obstacle || collisionData->pGO->m_tag == T_DeadEnemy || collisionData->pGO->m_tag == T_BoundaryObstacle)
			m_pTransform->SetPosition(m_pTransform->GetPosition() + collisionData->mtv.normal*collisionData->mtv.penetration);
	}
	else if (pEvent->Type() == EventType::EVENT_OnTakeDamage) {
		HealthChangeData* healthData = pEvent->Data<HealthChangeData>();
		if (healthData->mCurrentHealth == 0) {
			//m_pBody->SetVelocity(Vector3D(0, 0, 0));
			m_isDead = true;
		}
		else if (healthData->mIsForceKnockBack && m_knockBackMultiplier == 0) {
			//m_pBody->SetVelocity(Vector3D(0, 0, 0));
			pGO->HandleEvent(&Event(EVENT_ForceKnockBack));
			AddVelocity(healthData->m_directionOfAttack*healthData->mknockBackSpeed);
		}
		else {
			AddVelocity(healthData->m_directionOfAttack*m_knockBackMultiplier*healthData->mknockBackSpeed);
		}
	}
}

void Agent::LateInitialize() {
	if (!m_pTransform) {
		assert(pGO && "No Game Object found. Controller component failed to operate.");
		m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
		assert(m_pTransform && "No Transform component found. Controller component failed to operate.");
	}

	//if (!m_pBody) {
	//	assert(pGO && "No Game Object found. Controller component failed to operate.");
	//	m_pBody = pGO->GetComponent<Body>(ComponentType::C_Body);
	//	assert(m_pBody && "No Body component found. Controller component failed to operate.");
	//}

	if (!m_pAnimation) {
		if (pGO)
			m_pAnimation = pGO->GetComponent<Animation>(ComponentType::C_Animation);
	}
}

Vector3D Agent::GetDirectionFromPlayerToMouse() {
	Vector3D mousePos = Vector3D((float)TETRA_INPUT.MousePosX(), (float)TETRA_INPUT.MousePosY(), 0);
	GameObject* mainCam = TETRA_GAME_OBJECTS.GetPrimaryCamera();
	Camera* camComponent = mainCam->GetComponent<Camera>(ComponentType::C_Camera);
	const Transform* pPlayerTransform = TETRA_GAME_OBJECTS.GetPlayer()->GetComponent<Transform>(C_Transform);
	Vector3D screenSpace = camComponent->TransformPointToScreenSpace(pPlayerTransform->GetPosition());
	mousePos = mousePos*camComponent->GetZoom();
	Vector3D dirToMousePos = mousePos - screenSpace;
	dirToMousePos.y *= -1;
	dirToMousePos.Normalize();
	return dirToMousePos;
}