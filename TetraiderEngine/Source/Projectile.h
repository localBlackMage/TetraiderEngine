#pragma once
#ifndef PROJECTILE_H
#define PROJECTILE_H

//#include "Component.h"
//#include "Math\Vector3D.h"
//
//class Body;
//class Transform;
//class Event;

enum class ProjectileType {
	EnemyProjectile,
	PlayerProjectile,
	Neutral
};

class Projectile : public Component {
public:
	Projectile();
	~Projectile() {}
	static Component* CreateInstance() { return new Projectile(); }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt) {}
	virtual void HandleEvent(Event* pEvent);
	void SetProperties(int damage, float speed, const Vector3D& dir, float lifeTime, bool m_enemyProjectile, float knockBackSpeed, GameObject* owner);
	void SetProperties(int damage, float speed, const Vector3D& dir, float lifeTime, float knockBackSpeed, GameObject* owner);
private:
	Body* m_pBody;
	Transform* m_pTransform;
	GameObject* m_pOwner;
	ProjectileType m_projectileType;
	int m_damage;
	float m_lifeTime;
	float m_currentLifeTime;
	float m_knockBackSpeed;
	bool m_isEnemyProjectile;
	float m_deccelerationSpeed;
	bool m_isAccelerateToZero;
	bool m_isRotate;
	bool m_isIgnoreCollideEvent;
	bool m_isExplodeOnVelocityZero;
	bool m_isBouncy;
	bool m_isSpawnAcidPool;
	std::string m_acidPrefab;
};

#endif
