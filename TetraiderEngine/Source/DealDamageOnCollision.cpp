//#include "GameObject.h"
//#include "DealDamageOnCollision.h"
//#include "Health.h"
//#include "TetraiderAPI.h"
//#include "Controller.h"
//#include "NPCController.h"

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
}

void DealDamageOnCollision::LateInitialize() {}

void DealDamageOnCollision::HandleEvent(Event* pEvent) {
	if (TETRA_GAME_STATE.IsGamePaused()) return;

	if (pGO->m_tag == T_Hazard) {
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
				if (agent->GetIgnoreHazard())
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