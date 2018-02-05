#pragma once
#ifndef ATTACK_H
#define ATTACK_H

#include "Math/Vector3D.h"

class Weapon;

enum class AttackType {
	Melee,
	Ranged,

	None
};

class Attack {
public:
	Attack(float coolDown, int baseDamage, AttackType type): m_coolDown(coolDown), m_baseDamage(baseDamage), m_type(type) {}
	virtual ~Attack() {}
	virtual bool Use(const Vector3D& direction); // Assumes direction is normalized
	virtual void Update(float dt);
	virtual void Debug(const Vector3D& dir) {}
	bool IsOnCoolDown() { return m_isOnCooldown; }
	AttackType GetType() { return m_type; }
	friend Weapon;
protected:
	AttackType m_type;
	float m_coolDown;
	float m_lastUsedTimeStamp;
	int m_baseDamage;
	bool m_isOnCooldown;
	bool m_isAttacking;
	Weapon* m_pOwner;
};


#endif // !ATTACK_H
