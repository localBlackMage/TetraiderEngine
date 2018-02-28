#pragma once
#ifndef DEAL_DAMAGE_ON_COLLISION_H
#define DEAL_DAMAGE_ON_COLLISION_H

#include "Component.h"

class DealDamageOnCollision : public Component {
public:
	DealDamageOnCollision();
	~DealDamageOnCollision();
	static Component* CreateInstance() { return new DealDamageOnCollision(); }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt) {}
	virtual void HandleEvent(Event* pEvent);
private:
	int m_damage;
	float m_knockBackSpeed;
	bool m_isForceKnockBack;
	bool m_isAbleToDmgPlayer; // Dummy, applies for player only at the moment
	float m_hitCoolDown; // Dummy, applies for player only at the moment
	float m_timeFromLastHit; // Dummy, applies for player only at the moment
};

#endif

