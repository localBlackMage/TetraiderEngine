#pragma once
#ifndef PROJECTILE_SPAWNER_H
#define PROJECTILE_SPAWNER_H

#include "Component.h"
#include "Math/MathLibs.h"

class Weapon;

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
	Weapon* m_pWeapon;
};

#endif

