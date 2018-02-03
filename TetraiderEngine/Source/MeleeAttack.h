#pragma once
#ifndef MELEE_ATTACK_H
#define MELEE_ATTACK_H

#include "Attack.h"
#include "Math\Vector3D.h"

class MeleeAttack : public Attack {
public:
	MeleeAttack(float coolDown, int baseDamage, float radius, float angle, float triggerAttackIn);
	~MeleeAttack() {}
	void Update(float dt);
	bool Use(const Vector3D& direction);
private:
	void Run();
	float m_radius;
	float m_angle;
	float m_triggerAttackIn;
	Vector3D m_dirToAttackIn;
};

#endif 