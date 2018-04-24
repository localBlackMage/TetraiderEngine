/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

CamFollow::CamFollow() :
	Component(ComponentType::C_CamFollow),
	m_followSpeed(0.0f),
	m_isActive(true),
	m_isGoToBossRoom(false),
	m_showTime(0.0f)
{
	TETRA_EVENTS.Subscribe(EVENT_DisableCamFollow, this);
	TETRA_EVENTS.Subscribe(EVENT_EnableCamFollow, this);
	TETRA_EVENTS.Subscribe(EVENT_OnCamGoToBossRoom, this);
	TETRA_EVENTS.Subscribe(EVENT_OnCamGoToPlayer, this);
}

CamFollow::~CamFollow() {}

void CamFollow::Deactivate() { 
	pGO = nullptr; 
	m_pTransform = nullptr;  
	m_pTarget = nullptr; 
}

void CamFollow::Serialize(const json& j) {
	m_followSpeed = ParseFloat(j, "followSpeed");
	m_showSpeed = ParseFloat(j, "showSpeed");
	m_defaultTargetTag = ParseString(j, "defaultTargetTag");
}

void CamFollow::LateInitialize()
{
	if (!m_pTransform) {
		if (pGO)
			m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
		else {
			printf("No Game Object found. Camfollow component failed to operate.\n");
			return;
		}

		if (!m_pTransform) {
			printf("No Transform component found. Camfollow component failed to operate.\n");
			assert(m_pTransform);
			return;
		}
	}
}

void CamFollow::Update(float dt) {}

void CamFollow::LateUpdate(float dt) {
	if (TETRA_GAME_STATE.IsGamePaused()) return;

	if (m_isActive) {
		Transform* targetTransform = m_pTarget->GetComponent<Transform>(ComponentType::C_Transform);
		float z = m_pTransform->GetPosition().z;
		Vector3D lerpPosition = Lerp(m_pTransform->GetPosition(), targetTransform->GetPosition(), m_followSpeed*dt);
		m_pTransform->SetPosition(Vector3D(lerpPosition.x, lerpPosition.y, z));
	}

	if (m_isGoToBossRoom) {
		Transform* targetTransform = m_pTarget->GetComponent<Transform>(ComponentType::C_Transform);
		float z = m_pTransform->GetPosition().z;
		m_showTime += m_showSpeed*dt;
		Vector3D lerpPosition = Lerp(targetTransform->GetPosition(), bossTarget, m_showTime);
		m_pTransform->SetPosition(Vector3D(lerpPosition.x, lerpPosition.y, z));
		if (m_showTime > 1) {
			Event* pEvent = new Event(EVENT_OnOpenBossRoom, 0.3f);
			TETRA_EVENTS.AddDelayedEvent(pEvent);
			pEvent = new Event(EVENT_OnCamGoToPlayer, 1.8f);
			TETRA_EVENTS.AddDelayedEvent(pEvent);
			m_isGoToBossRoom = false;
			m_showTime = 0;
		}
	}
}

void CamFollow::HandleEvent(Event* pEvent) {
	if (pEvent->Type() == EVENT_OnLevelInitialized) {
		m_pTarget = TETRA_GAME_OBJECTS.FindObjectWithTag(TETRA_GAME_OBJECTS.FindTagWithString(m_defaultTargetTag));
		if (m_pTarget) {
			Transform* targetTransform = m_pTarget->GetComponent<Transform>(ComponentType::C_Transform);
			float z = m_pTransform->GetPosition().z;
			m_pTransform->SetPosition(Vector3D(targetTransform->GetPosition().x, targetTransform->GetPosition().y, z));
		}

		if (TETRA_LEVELS.IsBossLevel()) {
			bossTarget = TETRA_GAME_OBJECTS.FindObjectWithTag(T_TriggerBoxBoss)->GetComponent<Transform>(C_Transform)->GetPosition();
		}
	}
	else if (pEvent->Type() == EVENT_DisableCamFollow)
		m_isActive = false;
	else if (pEvent->Type() == EVENT_EnableCamFollow)
		m_isActive = true;
	else if (pEvent->Type() == EVENT_OnCamGoToBossRoom) {
		m_isActive = false;
		m_isGoToBossRoom = true;
	}
	else if (pEvent->Type() == EVENT_OnCamGoToPlayer) {
		m_isActive = true;
		m_isGoToBossRoom = false;
		Transform* targetTransform = m_pTarget->GetComponent<Transform>(ComponentType::C_Transform);
		float z = m_pTransform->GetPosition().z;
		m_pTransform->SetPosition(Vector3D(targetTransform->GetPosition().x, targetTransform->GetPosition().y, z));
	}
}