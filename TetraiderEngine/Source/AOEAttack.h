#pragma once
#ifndef AOE_ATTACK_H
#define AOE_ATTACK_H

#include "Attack.h"
#include "Math\Vector3D.h"

class GameObject;

class AOEAttack : public Attack {
public:
	AOEAttack(float coolDown, int baseDamage, float knockBackSpeed, AttackType type, float radius);
	~AOEAttack();
	void Update(float dt);
	bool Use(const Vector3D& direction);
	virtual void Debug(const Vector3D& dir) {}
private:
	float m_radius;
};

#endif 
