//#include "RangeAttack.h"
//#include "TetraiderAPI.h"
//#include "Weapon.h"
//#include "Transform.h"
//#include "Projectile.h"
//#include "GameObject.h"

#include <Stdafx.h>

RangeAttack::RangeAttack(float coolDown, int baseDamage, float knockBackSpeed, AttackType type, float projectileSpeed, float offset, float lifeTime, std::string projectilePrefab):
	Attack(coolDown, baseDamage, knockBackSpeed, type),
	m_projectileSpeed(projectileSpeed),
	m_offset(offset),
	m_lifeTime(lifeTime),
	m_projectilePrefab(projectilePrefab)
{}

RangeAttack::~RangeAttack() {}

// Assumes direction is normalized
bool RangeAttack::Use(const Vector3D& direction) {
	if (!Attack::Use(direction)) return false;

	Transform* pTransform = m_pOwner->pGO->GetComponent<Transform>(ComponentType::C_Transform);
	Vector3D instantiatePos = pTransform->GetPosition() + m_offset*direction;
	GameObject* pProjectileGO = TETRA_GAME_OBJECTS.CreateGameObject(m_projectilePrefab);
	Projectile* pProjectile = pProjectileGO->GetComponent<Projectile>(ComponentType::C_Projectile);

	bool isEnemyProjectile = false;
	if (m_pOwner->pGO->m_tag != T_Player)
		isEnemyProjectile = true;

	// TODO: Change base damage to take into consideration character stats
	pProjectile->SetProperties(instantiatePos, m_baseDamage, m_projectileSpeed, direction, m_lifeTime, isEnemyProjectile, m_knockBackSpeed, m_pOwner->pGO);
	return true;
}

void RangeAttack::Update(float dt) {
	Attack::Update(dt);
}