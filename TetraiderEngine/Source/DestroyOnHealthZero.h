#pragma once
#ifndef DESTROY_ONHEALTH_ZERO_H
#define DESTROY_ONHEALTH_ZERO_H

class Sprite;

#include "Component.h"
#include "Math/MathLibs.h"

class Attack;

class DestroyOnHealthZero : public Component {
public:
	DestroyOnHealthZero();
	~DestroyOnHealthZero();
	static Component* CreateInstance() { return new DestroyOnHealthZero(); }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt) {}
	virtual void HandleEvent(Event* pEvent);
private:
	float m_destroyIn;
	float m_timeStamp;
	bool m_isDestory;
	bool m_isExplode;
	float m_explosionRadius;
	float m_knockBackSpeed;
	int m_explosionDamage;
	std::string m_explosionPrefab;
	Attack* m_Attack;
};

#endif

