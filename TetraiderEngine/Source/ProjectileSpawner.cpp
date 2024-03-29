/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

ProjectileSpawner::ProjectileSpawner(): Component(ComponentType::C_ProjectileSpawner), m_diriection(Vector3D()), m_isActive(true), m_timeFromLastFire(0) {}
ProjectileSpawner::~ProjectileSpawner() {}

void ProjectileSpawner::DeActivate() {
	pGO = nullptr;
}

void ProjectileSpawner::Update(float dt) {
	if (TETRA_GAME_STATE.IsGamePaused()) return;

	//TETRA_DEBUG.DrawWireCircle(m_pTransform->GetPosition(), 2000, DebugColor::GREY);
	if (m_isActive) {
		m_timeFromLastFire += dt;
		if (m_timeFromLastFire > m_coolDown) {
			m_timeFromLastFire = 0;

			/*
			const GameObject* pPlayer = TETRA_GAME_OBJECTS.GetPlayer();
			const Transform* pPlayerTransform = pPlayer->GetComponent<Transform>(C_Transform);
			// If player is 800 pixels away do not fire

			if (Vector3D::SquareDistance(pPlayerTransform->GetPosition(), m_pTransform->GetPosition()) > 640000) return;*/

			Vector3D instantiatePos = m_pTransform->GetPosition() + m_instantiationOffset*m_diriection;
			GameObject* pProjectileGO = TETRA_GAME_OBJECTS.CreateGameObject(m_projectilePrefab, true, instantiatePos);
			Projectile* pProjectile = pProjectileGO->GetComponent<Projectile>(ComponentType::C_Projectile);
			bool isEnemyProjectile = true;
			pProjectile->SetProperties(m_baseDamage, m_projectileSpeed, m_diriection, m_lifeTime, m_knockBackSpeed, pGO);
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
		m_timeFromLastFire = m_timeOffset;
	}
}