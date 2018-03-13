//#include "GameObject.h"
//#include "Health.h"
//#include "TetraiderAPI.h"

#include <Stdafx.h>

Health::Health(): Component(ComponentType::C_Health) {}
Health::~Health() {}

void Health::Deactivate() { 
	pGO = nullptr; 
}

void Health::Update(float dt) {}

void Health::Serialize(const json& j) {
	m_currentHealth = ParseInt(j, "startingHealth");
	m_maxHealth = ParseInt(j, "maxHealth");
	m_isInvincible = ParseBool(j, "isInvincible");
}

void Health::LateInitialize() {}
void Health::HandleEvent(Event* pEvent) {
	switch (pEvent->Type()) {
		case EVENT_HealthCollected: {
			CollectibleData* pData = pEvent->Data<CollectibleData>();
			Heal(pData->m_value);
			break;
		}
		case EVENT_OnLevelInitialized: {
			if (pGO->m_tag == T_Player) {
				int extraHealth = TETRA_PLAYERSTATS.GetHealthUpgrade();
				UpdgradeMaxHealth(extraHealth);
			}
		}
	}
}

void Health::TakeDamage(int damage, const Vector3D& directionOfAttack, float knockBackSpeed, bool isForceKnockBack) {
	if (m_isInvincible || m_currentHealth == 0)
		return;

	m_currentHealth -= damage;

	if (m_currentHealth <= 0) {
		m_currentHealth = 0;
		if (pGO->m_tag == GameObjectTag::T_Player) {
			TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EventType::EVENT_OnPlayerHealthZero));
			// For prototype deactivate player here
			pGO->SetActive(false);
		}
		else if (pGO->m_tag == GameObjectTag::T_Enemy) {
			TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EventType::EVENT_OnEnemyHealthZero));
			// For prototype just destroy the enemy here
			pGO->Destroy();
		}

		pGO->HandleEvent(&Event(EventType::EVENT_OnHealthZero));
	}

	Event e = Event(EventType::EVENT_OnTakeDamage, &HealthChangeData(m_currentHealth, m_maxHealth, directionOfAttack, knockBackSpeed, isForceKnockBack));
	pGO->HandleEvent(&e);
	if (pGO->m_tag == GameObjectTag::T_Player) {
		TETRA_EVENTS.BroadcastEventToSubscribers(&e);
	}
}

void Health::Heal(int heal) {
	m_currentHealth += heal;
	if (m_currentHealth > m_maxHealth) {
		m_currentHealth = m_maxHealth;
	}

	if (pGO->m_tag == GameObjectTag::T_Player) {
		TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EventType::EVENT_OnPlayerHeal,&HealthChangeData(m_currentHealth, m_maxHealth, Vector3D(), 0, false)));
	}
}

bool Health::IsHealthFull() { return m_currentHealth == m_maxHealth; }
void Health::UpdgradeMaxHealth(int value) { m_maxHealth += value; }

void Health::SetHealth(int currentHealth, int maxHealth) {
	m_maxHealth = maxHealth;
	m_currentHealth = currentHealth;

	if (m_currentHealth > m_maxHealth)
		m_currentHealth = m_maxHealth;

	if (pGO->m_tag == GameObjectTag::T_Player) {
		TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EventType::EVENT_OnPlayerHeal, &HealthChangeData(m_currentHealth, m_maxHealth, Vector3D(), 0, false)));
	}
}