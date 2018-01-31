#include "Health.h"

Health::Health(): Component(ComponentType::Health) {}
Health::~Health() {}

void Health::Update(float dt) {}

void Health::Serialize(json j) {
	m_currentHealth = ParseInt(j, "startingHealth");
	m_maxHealth = ParseInt(j, "maxHealth");
	m_isInvincible = ParseBool(j, "isInvincible");
}

void Health::LateInitialize() {}
void Health::HandleEvent(Event* pEvent) {}

void Health::TakeDamage(int damage) {
	if (m_isInvincible || m_currentHealth == 0)
		return;

	m_currentHealth -= damage;

	if (m_currentHealth <= 0) {
		m_currentHealth = 0;
		if (pGO->m_tag == GameObjectTag::Player) {
			OnPlayerHealthZero onPlayerHealthZero;
			T_EVENTS.BroadcastEventToSubscribers(&onPlayerHealthZero);
		}
		else if (pGO->m_tag == GameObjectTag::Enemy) {
			OnEnemyHealthZero onEnemyHealthZero;
			T_EVENTS.BroadcastEventToSubscribers(&onEnemyHealthZero);
		}
	}

	OnTakeDamage onTakeDamage;
	onTakeDamage.currentHealth = m_currentHealth;
	onTakeDamage.maxHealth = m_maxHealth;
	pGO->HandleEvent(&onTakeDamage);
}

void Health::Heal(int heal) {
	m_currentHealth += heal;
	if (m_currentHealth > m_maxHealth) {
		m_currentHealth = m_maxHealth;
	}

	if (pGO->m_tag == GameObjectTag::Player) {
		OnPlayerHeal onPlayerHeal;
		onPlayerHeal.currentHealth;
		onPlayerHeal.maxHealth;
		T_EVENTS.BroadcastEventToSubscribers(&onPlayerHeal);
	}
}

bool Health::IsHealthFull() { return m_currentHealth == m_maxHealth; }
void Health::UpdgradeMaxHealth(int value) { m_maxHealth += value; }

