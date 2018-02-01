#pragma once
#ifndef RANGE_ATTACK_H
#define RANGE_ATTACK_H

#include "Attack.h"
#include <string>

class RangeAttack : public Attack {
public:
	RangeAttack();
	~RangeAttack();
private:
	std::string projectilePrefab;
	float m_speed;
};

#endif // !RANGE_ATTACK_H
