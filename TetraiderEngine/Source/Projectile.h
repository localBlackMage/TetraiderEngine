#pragma once
#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Component.h"

class Body;
class Event;

class Projectile : public Component {
public:
	Projectile(): Component(C_Projectile) {}
	~Projectile() {}
	virtual void Update(float dt) {}
	virtual void Serialize(json j) {}
	virtual void LateInitialize() {}
	virtual void LateUpdate(float dt) {}
	virtual void HandleEvent(Event* pEvent) {}
private:
	Body* m_pBody;
	float m_damage;
};

#endif
