#pragma once
#ifndef ATTACK_H
#define ATTACK_H

#include "Math/Vector3D.h"

class Weapon;

class Attack {
public:
	Attack(float coolDown, int baseDamage): m_coolDown(coolDown), m_baseDamage(baseDamage) {}
	virtual ~Attack() {}
	virtual bool Use(const Vector3D& direction); // Assumes direction is normalized
	virtual void Update(float dt);
	bool IsOnCoolDown() { return m_isOnCooldown; }
	friend Weapon;
protected:
	float m_coolDown;
	float m_lastUsedTimeStamp;
	int m_baseDamage;
	bool m_isOnCooldown;
	bool m_isAttacking;
	Weapon* m_pOwner;
};


#endif // !ATTACK_H
