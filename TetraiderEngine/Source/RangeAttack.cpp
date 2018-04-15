#include <Stdafx.h>

RangeAttack::RangeAttack(float coolDown, int baseDamage, float knockBackSpeed, int ammo, bool isUnlimitedAmmo, AttackType type, float projectileSpeed, float offset, float lifeTime, int multipleShot, float coneAngle, bool isFlash, std::string projectilePrefab):
	Attack(coolDown, baseDamage, knockBackSpeed, type),
	m_projectileSpeed(projectileSpeed),
	m_offset(offset),
	m_lifeTime(lifeTime),
	m_projectilePrefab(projectilePrefab),
	m_ammo(ammo),
	m_isUnlimitedAmmo(isUnlimitedAmmo),
	m_multipleShot(multipleShot),
	m_coneAngle(coneAngle),
	m_isFlash(isFlash)
{
}

RangeAttack::~RangeAttack() {}

// Assumes direction is normalized
bool RangeAttack::Use(const Vector3D& direction) {
	if (!Attack::Use(direction)) return false;
	int temp = m_multipleShot;
	if (!m_isUnlimitedAmmo) {
		if (m_ammo == 0) return false; // out of ammo

		m_ammo -= m_multipleShot;
		if (m_ammo < 0) {
			m_multipleShot = m_ammo + m_multipleShot;
			m_ammo = 0;
		}
		TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EventType::EVENT_UIAmmoUpdate, &CollectibleData(m_ammo)));
	}

	Transform* pTransform = m_pOwner->pGO->GetComponent<Transform>(ComponentType::C_Transform);
	bool isEnemyProjectile = false;
	if (m_pOwner->pGO->m_tag != T_Player)
		isEnemyProjectile = true;

	if (m_multipleShot <= 1) {
		Vector3D instantiatePos = pTransform->GetPosition() + m_offset*direction;
		GameObject* pProjectileGO = TETRA_GAME_OBJECTS.CreateGameObject(m_projectilePrefab, true, instantiatePos);
		Projectile* pProjectile = pProjectileGO->GetComponent<Projectile>(ComponentType::C_Projectile);
		// TODO: Change base damage to take into consideration character stats
		pProjectile->SetProperties(m_baseDamage, m_projectileSpeed, direction, m_lifeTime, isEnemyProjectile, m_knockBackSpeed, m_pOwner->pGO);

		if (m_isFlash) {
			m_pOwner->HideWeapon(true);
			m_isWeaponFlash = true;
		}
	}
	else {
		//float angle = Vector3D
		float angle = direction.AngleDegrees() + m_coneAngle;
		if (angle > 180.0f)	angle -= 360;
		else if (angle < -180) angle += 360;

		float offset = m_coneAngle / (m_multipleShot - 1);
		for (int i = 0; i < m_multipleShot; ++i) {
			float a = angle - offset*i;
			if (a > 180.0f)	a -= 360;
			else if (a < -180) a += 360;
			Vector3D dir = Vector3D::VectorFromAngleDegrees(a - offset*i);
			Vector3D instantiatePos = pTransform->GetPosition() + m_offset*dir;
			GameObject* pProjectileGO = TETRA_GAME_OBJECTS.CreateGameObject(m_projectilePrefab, true, instantiatePos);
			Projectile* pProjectile = pProjectileGO->GetComponent<Projectile>(ComponentType::C_Projectile);
			pProjectile->SetProperties(m_baseDamage*m_pOwner->GetDamageMultiplier(), m_projectileSpeed, dir, m_lifeTime, isEnemyProjectile, m_knockBackSpeed, m_pOwner->pGO);
		}

		if (m_isFlash) {
			m_pOwner->HideWeapon(true);
			m_isWeaponFlash = true;
		}
	}

	m_multipleShot = temp;
		
	return true;
}

void RangeAttack::Update(float dt) {
	Attack::Update(dt);
}