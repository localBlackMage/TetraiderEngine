#pragma once
#ifndef PROJECTILE_SPAWNER_H
#define PROJECTILE_SPAWNER_H

#include "Component.h"
#include "Math/MathLibs.h"

class Transform;

class ProjectileSpawner : public Component {
public:
	ProjectileSpawner();
	~ProjectileSpawner();
	static Component* CreateInstance() { return new ProjectileSpawner(); }
	virtual void DeActivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt) {}
	virtual void HandleEvent(Event* pEvent);
	void Active(bool active) { m_isActive = active; }
	void SetDirection(const Vector3D direction) { m_diriection = direction; }
private:
	bool m_isActive;
	Vector3D m_diriection;
	float m_coolDown;
	int m_baseDamage;
	float m_projectileSpeed;
	float m_instantiationOffset;
	float m_lifeTime;
	std::string m_projectilePrefab;
	float m_knockBackSpeed;
	float m_lastFiredTimeStamp;
	float m_timeOffset;
	Transform* m_pTransform;
};

#endif

