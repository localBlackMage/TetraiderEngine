#pragma once
#ifndef RANGE_ATTACK_H
#define RANGE_ATTACK_H

#include "Attack.h"
#include "Math\Vector3D.h"
#include <string>

class RangeAttack : public Attack {
public:
	RangeAttack(float coolDown, int baseDamage, float knockBackSpeed, AttackType type, float projectileSpeed, float offset, float lifeTime, std::string projectilePrefab);
	~RangeAttack();
	virtual bool Use(const Vector3D& direction); // Assumes direction is normalized
	virtual void Update(float dt);
private:
	std::string m_projectilePrefab;
	float m_projectileSpeed;
	float m_offset;
	float m_lifeTime;
};

#endif // !RANGE_ATTACK_H
