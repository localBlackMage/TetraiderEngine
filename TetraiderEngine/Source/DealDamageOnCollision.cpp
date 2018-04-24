/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

DealDamageOnCollision::DealDamageOnCollision(): Component(ComponentType::C_DealDamageOnCollision), m_hitCoolDown(0.25f), m_isAbleToDmgPlayer(true), m_timeFromLastHit(0.0f) {}
DealDamageOnCollision::~DealDamageOnCollision() {}

void DealDamageOnCollision::Update(float dt) {
	if (TETRA_GAME_STATE.IsGamePaused()) return;

	if (!m_isAbleToDmgPlayer) {
		m_timeFromLastHit += dt;
		if (m_timeFromLastHit > m_hitCoolDown) {
			m_isAbleToDmgPlayer = true;
		}
	}
}

void DealDamageOnCollision::Deactivate() {
	pGO = nullptr;
	m_isAbleToDmgPlayer = true, 
	m_timeFromLastHit = 0.0f;
}

void DealDamageOnCollision::Serialize(const json& j) {
	m_damage = ParseInt(j, "damage");
	m_knockBackSpeed = ParseFloat(j, "knockBackSpeed");
	m_isForceKnockBack = ParseBool(j, "isForceKnockBack");
	m_hitCoolDown = ParseFloat(j, "hitCoolDown");
}

void DealDamageOnCollision::LateInitialize() {}

void DealDamageOnCollision::HandleEvent(Event* pEvent) {
	if (TETRA_GAME_STATE.IsGamePaused()) return;

	if (pGO->m_tag == T_Hazard || pGO->m_tag == T_AcidHazard) {
		if (pEvent->Type() == EVENT_OnCollide) {
			OnCollideData* collisionData = pEvent->Data<OnCollideData>();
			if (collisionData->pGO->m_tag == T_Player) {
				if (!m_isAbleToDmgPlayer)
					return;

				Agent* agent = collisionData->pGO->GetComponent<Controller>(C_Controller);
				m_timeFromLastHit = 0.0f;
				if (agent->GetIgnoreHazard())
					return;

				m_isAbleToDmgPlayer = false;
			}
			else if (collisionData->pGO->m_tag == T_Enemy) {
				Agent* agent = collisionData->pGO->GetComponent<Controller>(C_NPCCONTROLLER);
				if (!m_isAbleToDmgPlayer)
					return;

				m_timeFromLastHit = 0.0f;
				if (agent->GetIgnoreHazard() || (agent->GetIgnoreAcid() && pGO->m_tag == T_AcidHazard))
					return;

				m_isAbleToDmgPlayer = false;
			}

			if (collisionData->pGO->m_tag == T_Obstacle && pGO->m_tag == T_AcidHazard) {
				DestroyOnHealthZero* pDestroyOnHealthZero = collisionData->pGO->GetComponent<DestroyOnHealthZero>(C_DestroyOnHealthZero);
				if (pDestroyOnHealthZero)
					return;
			}

			Health* pHealth = collisionData->pGO->GetComponent<Health>(C_Health);
			if (pHealth) {
				pHealth->TakeDamage(m_damage, collisionData->mtv.normal*-1, m_knockBackSpeed, m_isForceKnockBack);
			}
		}
	}
	else if (pGO->m_tag == T_Enemy) {
		if (pEvent->Type() == EVENT_OnCollide) {
			OnCollideData* collisionData = pEvent->Data<OnCollideData>();
			if (collisionData->pGO->m_tag == T_Player) {
				if (!m_isAbleToDmgPlayer)
					return;

				m_timeFromLastHit = 0.0f;
				m_isAbleToDmgPlayer = false;
				Health* pHealth = collisionData->pGO->GetComponent<Health>(C_Health);
				if (pHealth) {
					pHealth->TakeDamage(m_damage, collisionData->mtv.normal*-1, m_knockBackSpeed, m_isForceKnockBack);
				}
			}
		}
	}
}