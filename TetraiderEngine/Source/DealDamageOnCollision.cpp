#include "GameObject.h"
#include "DealDamageOnCollision.h"
#include "Health.h"
#include "TetraiderAPI.h"
#include "Controller.h"
#include "NPCController.h"

DealDamageOnCollision::DealDamageOnCollision(): Component(ComponentType::C_DealDamageOnCollision), m_hitCoolDown(0.15f), m_isAbleToDmgPlayer(true), m_timeStampOfLastHitPlayer(0.0f) {}
DealDamageOnCollision::~DealDamageOnCollision() {}

void DealDamageOnCollision::Update(float dt) {
	if (!m_isAbleToDmgPlayer) {
		if (TETRA_FRAMERATE.GetElapsedTime() - m_timeStampOfLastHitPlayer > m_hitCoolDown) {
			m_isAbleToDmgPlayer = true;
		}
	}
}

void DealDamageOnCollision::DeActivate() {
	pGO = nullptr; 
	m_isAbleToDmgPlayer = true, 
	m_timeStampOfLastHitPlayer = 0.0f; 
}

void DealDamageOnCollision::Serialize(const json& j) {
	m_damage = ParseInt(j, "damage");
	m_knockBackSpeed = ParseFloat(j, "knockBackSpeed");
}

void DealDamageOnCollision::LateInitialize() {}

void DealDamageOnCollision::HandleEvent(Event* pEvent) {
	if (pEvent->Type() == EVENT_OnCollide) {
		OnCollideData* collisionData = pEvent->Data<OnCollideData>();
		if (collisionData->pGO->m_tag == T_Player) {
			if (!m_isAbleToDmgPlayer)
				return;

			Agent* agent = collisionData->pGO->GetComponent<Controller>(C_Controller);
			m_timeStampOfLastHitPlayer = TETRA_FRAMERATE.GetElapsedTime();
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
			pHealth->TakeDamage(m_damage, collisionData->mtv.normal*-1, m_knockBackSpeed);
		}
	}
}