/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef HEALTH_H
#define HEALTH_H

class Health : public Component {
public:
	Health();
	~Health();
	static Component* CreateInstance() { return new Health(); }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt) {}
	virtual void HandleEvent(Event* pEvent);

	bool IsHealthFull();
	void UpdgradeMaxHealth(int value);
	void TakeDamage(int damage, const Vector3D& sourceOfAttack, float knockBackSpeed, bool isForceKnockBack);
	void Heal(int heal);
	int GetHealth() const { return m_currentHealth; }
	int GetMaxHealth() const { return m_maxHealth; }
	void SetHealth(int currentHealth, int maxHealth);
	void Invincibility(bool active) { m_isInvincible = active; }
private:
	int m_currentHealth;
	int m_maxHealth;
	bool m_isInvincible;
};

#endif

