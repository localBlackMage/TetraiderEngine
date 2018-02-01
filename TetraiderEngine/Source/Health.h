#ifndef HEALTH_H
#define HEALTH_H

#include "Component.h"

class Health : public Component {
public:
	Health();
	~Health();
	virtual void Update(float dt);
	virtual void Serialize(json j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt) {}
	virtual void HandleEvent(Event* pEvent);

	bool IsHealthFull();
	void UpdgradeMaxHealth(int value);
	void TakeDamage(int damage);
	void Heal(int heal);
	int GetHealth() { return m_currentHealth; }
	void Invincibility(bool active) { m_isInvincible = active; }
private:
	int m_currentHealth;
	int m_maxHealth;
	bool m_isInvincible;
};

class OnEnemyHealthZero :public Event {
public:
	OnEnemyHealthZero(): Event(EVENT_OnEnemyHealthZero) { }
	~OnEnemyHealthZero() {}
};

class OnPlayerHealthZero :public Event {
public:
	OnPlayerHealthZero(): Event(EVENT_OnPlayerHealthZero) {}
	~OnPlayerHealthZero() {}
};

class OnTakeDamage :public Event {
public:
	OnTakeDamage(): Event(EVENT_OnTakeDamage) {}
	~OnTakeDamage() {}
	int currentHealth;
	int maxHealth;
};

class OnPlayerHeal :public Event {
public:
	OnPlayerHeal(): Event(EVENT_OnPlayerHeal) {}
	~OnPlayerHeal() {}
	int currentHealth;
	int maxHealth;
};

#endif

