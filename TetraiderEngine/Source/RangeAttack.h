#pragma once
#ifndef RANGE_ATTACK_H
#define RANGE_ATTACK_H

//#include "Attack.h"
//#include "Math\Vector3D.h"
//#include <string>

class RangeAttack : public Attack {
public:
	RangeAttack::RangeAttack(float coolDown, int baseDamage, float knockBackSpeed, int ammo, bool isUnlimitedAmmo, AttackType type, float projectileSpeed, float offset, float lifeTime, int multipleShot, float coneAngle, std::string projectilePrefab);
	~RangeAttack();
	virtual bool Use(const Vector3D& direction); // Assumes direction is normalized
	virtual void Update(float dt);
	int GetAmmo() { return m_ammo; }
	void UpdateAmmo(int ammo) { m_ammo = ammo; }
	void SetMultipleShot(int shots, float coneAngle) { m_multipleShot = shots; m_coneAngle = coneAngle; }
private:
	std::string m_projectilePrefab;
	float m_projectileSpeed;
	float m_offset;
	float m_lifeTime;
	int m_multipleShot;
	float m_coneAngle;
	int m_ammo;
	bool m_isUnlimitedAmmo;
};

#endif // !RANGE_ATTACK_H
