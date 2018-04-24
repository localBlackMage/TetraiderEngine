/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef ATTACK_H
#define ATTACK_H

//#include "Math/Vector3D.h"
class Weapon;

enum class AttackType {
	Melee,
	Ranged,
	AOE,

	None
};

class Attack {
public:
	Attack(float coolDown, int baseDamage, float knockBackSpeed, AttackType type): 
		m_coolDown(coolDown), m_baseDamage(baseDamage), m_knockBackSpeed(knockBackSpeed), m_type(type), m_isWeaponFlash(false) {}
	virtual ~Attack() {}
	virtual bool Use(const Vector3D& direction); // Assumes direction is normalized
	virtual void Update(float dt);
	virtual void Debug(const Vector3D& dir) {}
	void AddDamage(int damage) { m_baseDamage += damage; }
	void UpdateCoolDown(float multiplier);
	void MultiplyDamage(float multiplier);
	bool IsOnCoolDown() { return m_isOnCooldown; }
	float GetCoolDown() { return m_coolDown; }
	AttackType GetType() { return m_type; }
	friend Weapon;
protected:
	AttackType m_type;
	float m_coolDown;
	float m_timeFromLastUsed;
	float m_knockBackSpeed;
	int m_baseDamage;
	bool m_isOnCooldown;
	bool m_isAttacking;
	Weapon* m_pOwner;
	bool m_isWeaponFlash;
};


#endif // !ATTACK_H
