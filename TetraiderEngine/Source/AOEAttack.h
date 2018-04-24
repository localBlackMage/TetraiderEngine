/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef AOE_ATTACK_H
#define AOE_ATTACK_H

//#include "Attack.h"
//#include "Math\Vector3D.h"
//
//class GameObject;

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
