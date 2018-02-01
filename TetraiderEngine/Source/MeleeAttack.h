#pragma once
#ifndef MELEE_ATTACK_H
#define MELEE_ATTACK_H

#include "Attack.h"
#include "Math\Vector3D.h"

class MeleeAttack : public Attack {
public:
	MeleeAttack() {}
	~MeleeAttack() {}
	void Update(float dt);
	bool Use(const Vector3D& direction);
private:
	void Attack(float dt);
	float m_radius;
	float m_angle;
	float m_triggerAttackIn;
	Vector3D dirToAttackIn;
};

#endif 
