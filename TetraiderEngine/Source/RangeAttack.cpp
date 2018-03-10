//#include "RangeAttack.h"
//#include "TetraiderAPI.h"
//#include "Weapon.h"
//#include "Transform.h"
//#include "Projectile.h"
//#include "GameObject.h"

#include <Stdafx.h>

RangeAttack::RangeAttack(float coolDown, int baseDamage, float knockBackSpeed, int ammo, bool isUnlimitedAmmo, AttackType type, float projectileSpeed, float offset, float lifeTime, std::string projectilePrefab):
	Attack(coolDown, baseDamage, knockBackSpeed, type),
	m_projectileSpeed(projectileSpeed),
	m_offset(offset),
	m_lifeTime(lifeTime),
	m_projectilePrefab(projectilePrefab),
	m_ammo(ammo),
	m_isUnlimitedAmmo(isUnlimitedAmmo)
{}

RangeAttack::~RangeAttack() {}

// Assumes direction is normalized
bool RangeAttack::Use(const Vector3D& direction) {
	if (!Attack::Use(direction)) return false;
	if (!m_isUnlimitedAmmo) {
		if (m_ammo == 0) return false; // out of ammo
		--m_ammo;
		TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EventType::EVENT_UIAmmoUpdate, &CollectibleData(m_ammo)));
	}

	Transform* pTransform = m_pOwner->pGO->GetComponent<Transform>(ComponentType::C_Transform);
	Vector3D instantiatePos = pTransform->GetPosition() + m_offset*direction;
	GameObject* pProjectileGO = TETRA_GAME_OBJECTS.CreateGameObject(m_projectilePrefab, true, instantiatePos);
	Projectile* pProjectile = pProjectileGO->GetComponent<Projectile>(ComponentType::C_Projectile);

	bool isEnemyProjectile = false;
	if (m_pOwner->pGO->m_tag != T_Player)
		isEnemyProjectile = true;

	// TODO: Change base damage to take into consideration character stats
	pProjectile->SetProperties(m_baseDamage, m_projectileSpeed, direction, m_lifeTime, isEnemyProjectile, m_knockBackSpeed, m_pOwner->pGO);
	return true;
}

void RangeAttack::Update(float dt) {
	Attack::Update(dt);
}