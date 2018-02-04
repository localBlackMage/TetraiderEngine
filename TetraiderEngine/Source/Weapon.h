#pragma once
#ifndef WEAPON_H
#define WEAPON_H

#include "Component.h"

class Attack;

class Weapon : public Component {
public:
	Weapon();
	~Weapon();
	static Component* CreateInstance() { return new Weapon(); }
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt) {}
	virtual void HandleEvent(Event* pEvent);
	bool UseAttack(int attack, const Vector3D& dirToAttack);
private:
	std::vector<Attack*> m_Attacks;
};

#endif
