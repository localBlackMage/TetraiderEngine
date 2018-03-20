#pragma once
#ifndef MELEE_ATTACK_H
#define MELEE_ATTACK_H

class MeleeAttack : public Attack {
public:
	MeleeAttack(float coolDown, int baseDamage, float knockBackSpeed, AttackType type, float radius, float angle, float triggerAttackIn);
	~MeleeAttack();
	void Update(float dt);
	bool Use(const Vector3D& direction);
	void IncreaseRadius(float multiplier) { m_radius = m_radius*multiplier; }
	virtual void Debug(const Vector3D& dir);
private:
	void Run();
	float m_radius;
	float m_angle;
	float m_triggerAttackIn;
	Vector3D m_dirToAttackIn;
};

#endif 
