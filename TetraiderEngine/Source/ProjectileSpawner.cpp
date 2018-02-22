#include "GameObject.h"
#include "Transform.h"
#include "ProjectileSpawner.h"
#include "TetraiderAPI.h"
#include "Projectile.h"

ProjectileSpawner::ProjectileSpawner(): Component(ComponentType::C_ProjectileSpawner), m_diriection(Vector3D()), m_isActive(true), m_lastFiredTimeStamp(0) {}
ProjectileSpawner::~ProjectileSpawner() {}

void ProjectileSpawner::DeActivate() {
	pGO = nullptr;
}

void ProjectileSpawner::Update(float dt) {
	if (m_isActive) {
		if (TETRA_FRAMERATE.GetElapsedTime() - m_lastFiredTimeStamp > m_coolDown) {
			Vector3D instantiatePos = m_pTransform->GetPosition() + m_instantiationOffset*m_diriection;
			GameObject* pProjectileGO = TETRA_GAME_OBJECTS.CreateGameObject(m_projectilePrefab);
			Projectile* pProjectile = pProjectileGO->GetComponent<Projectile>(ComponentType::C_Projectile);
			bool isEnemyProjectile = true;
			pProjectile->SetProperties(instantiatePos, m_baseDamage, m_projectileSpeed, m_diriection, m_lifeTime, isEnemyProjectile, m_knockBackSpeed, pGO);
			m_lastFiredTimeStamp = TETRA_FRAMERATE.GetElapsedTime();
		}
	}
}

void ProjectileSpawner::Serialize(const json& j) {
	m_isActive = ParseBool(j, "isActive");
	m_diriection.x = ParseFloat(j["direction"], "x");
	m_diriection.y = ParseFloat(j["direction"], "y");
	m_diriection.z = ParseFloat(j["direction"], "z");
	m_coolDown = ParseFloat(j, "coolDown");
	m_baseDamage = ParseInt(j, "baseDamage");
	m_projectileSpeed = ParseFloat(j, "projectileSpeed");
	m_instantiationOffset = ParseFloat(j, "offset");
	m_lifeTime = ParseFloat(j, "lifeTime");
	m_projectilePrefab = ParseString(j, "projectilePrefab");
	m_knockBackSpeed = ParseFloat(j, "knockBackSpeed");
	m_timeOffset = ParseFloat(j, "timeOffset");
}

void ProjectileSpawner::LateInitialize() {
	if (!m_pTransform) {
		if (pGO)
			m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
		else {
			printf("No Game Object found. Projectile spawner component failed to operate.\n");
			return;
		}

		if (!m_pTransform) {
			printf("No Transform component found. Projectile spawner component failed to operate.\n");
			assert(m_pTransform);
			return;
		}
	}
}

void ProjectileSpawner::HandleEvent(Event* pEvent) {
	if (pEvent->Type() == EVENT_OnLevelInitialized) {
		m_lastFiredTimeStamp = TETRA_FRAMERATE.GetElapsedTime() + m_timeOffset;
	}
}