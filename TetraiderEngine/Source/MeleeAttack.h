#pragma once
#ifndef MELEE_ATTACK_H
#define MELEE_ATTACK_H

#include "Attack.h"
#include "Math\Vector3D.h"

class GameObject;

class MeleeAttack : public Attack {
public:
	MeleeAttack(float coolDown, int baseDamage, AttackType type, float radius, float angle, float triggerAttackIn);
	~MeleeAttack();
	void Update(float dt);
	bool Use(const Vector3D& direction);
	virtual void Debug(const Vector3D& dir);
private:
	void Run();
	float m_radius;
	float m_angle;
	float m_triggerAttackIn;
	Vector3D m_dirToAttackIn;

	// Dummy code
	GameObject* m_pSlashEffect;
};

#endif 
