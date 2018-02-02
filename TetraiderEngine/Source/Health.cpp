#include "GameObject.h"
#include "Health.h"
#include "TetraiderAPI.h"

Health::Health(): Component(ComponentType::C_Health) {}
Health::~Health() {}

void Health::Update(float dt) {}

void Health::Serialize(json j) {
	m_currentHealth = ParseInt(j, "startingHealth");
	m_maxHealth = ParseInt(j, "maxHealth");
	m_isInvincible = ParseBool(j, "isInvincible");
}

void Health::LateInitialize() {}
void Health::HandleEvent(Event* pEvent) {}

void Health::TakeDamage(int damage, const Vector3D& sourceOfAttack) {
	if (m_isInvincible || m_currentHealth == 0)
		return;

	m_currentHealth -= damage;

	if (m_currentHealth <= 0) {
		m_currentHealth = 0;
		if (pGO->m_tag == GameObjectTag::T_Player) {
			TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EventType::EVENT_OnPlayerHealthZero));
		}
		else if (pGO->m_tag == GameObjectTag::T_Enemy) {
			TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EventType::EVENT_OnEnemyHealthZero));
		}
	}

	pGO->HandleEvent(&Event(EventType::EVENT_OnTakeDamage, &HealthChangeData(m_currentHealth, m_maxHealth, sourceOfAttack)));
}

void Health::Heal(int heal) {
	m_currentHealth += heal;
	if (m_currentHealth > m_maxHealth) {
		m_currentHealth = m_maxHealth;
	}

	if (pGO->m_tag == GameObjectTag::T_Player) {
		TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EventType::EVENT_OnPlayerHeal,&HealthChangeData(m_currentHealth, m_maxHealth, Vector3D())));
	}
}

bool Health::IsHealthFull() { return m_currentHealth == m_maxHealth; }
void Health::UpdgradeMaxHealth(int value) { m_maxHealth += value; }