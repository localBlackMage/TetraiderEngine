#pragma once
#ifndef RANGE_ATTACK_H
#define RANGE_ATTACK_H

#include "Attack.h"
#include "Math\Vector3D.h"
#include <string>

class RangeAttack : public Attack {
public:
	RangeAttack();
	~RangeAttack();
	virtual bool Use(const Vector3D& direction); // Assumes direction is normalized
	virtual void Update(float dt);
private:
	std::string projectilePrefab;
	float m_speed;
	float m_offset;
	float m_lifeTime;
};

#endif // !RANGE_ATTACK_H
