#pragma once
#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Component.h"
#include "Math\Vector3D.h"

class Body;
class Transform;
class Event;

class Projectile : public Component {
public:
	Projectile();
	~Projectile() {}
	virtual void Update(float dt);
	virtual void Serialize(json j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt) {}
	virtual void HandleEvent(Event* pEvent);
	void SetProperties(const Vector3D& position, int damage, float speed, const Vector3D& dir, float lifeTime, bool m_enemyProjectile);
private:
	Body* m_pBody;
	Transform* m_pTransform;
	int m_damage;
	float m_lifeTime;
	float m_creationLifeTime;
	bool m_isEnemyProjectile;
};

#endif