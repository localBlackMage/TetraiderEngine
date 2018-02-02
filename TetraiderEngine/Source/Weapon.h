#pragma once
#ifndef WEAPON_H
#define WEAPON_H

#include "Component.h"

class Weapon : public Component {
public:
	Weapon(): Component(C_Weapon) {}
	~Weapon() {}
	virtual void Update(float dt) {}
	virtual void Serialize(json j) {}
	virtual void LateInitialize() {}
	virtual void LateUpdate(float dt) {}
	virtual void HandleEvent(Event* pEvent) {}
private:
};

#endif
