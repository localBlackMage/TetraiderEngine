#include "GameObject.h"
#include "Weapon.h"
#include "ProjectileSpawner.h"
#include "TetraiderAPI.h"

ProjectileSpawner::ProjectileSpawner(): Component(ComponentType::C_ProjectileSpawner), m_diriection(Vector3D()), m_isActive(true), m_pWeapon(nullptr) {}
ProjectileSpawner::~ProjectileSpawner() {}

void ProjectileSpawner::DeActivate() {
	m_pWeapon = nullptr;
}

void ProjectileSpawner::Update(float dt) {
	if (m_isActive)
		m_pWeapon->UseAttack(0, m_diriection);
}

void ProjectileSpawner::Serialize(const json& j) {
	m_isActive = ParseBool(j, "isActive");
	m_diriection.x = ParseFloat(j["direction"], "x");
	m_diriection.y = ParseFloat(j["direction"], "y");
	m_diriection.z = ParseFloat(j["direction"], "z");
}

void ProjectileSpawner::LateInitialize() {
	if (!m_pWeapon) {
		if (pGO)
			m_pWeapon = pGO->GetComponent<Weapon>(ComponentType::C_Weapon);
		else {
			printf("No Game Object found. Projectile spawner component failed to operate.\n");
			return;
		}

		if (!m_pWeapon) {
			printf("No Weapon component found. Projectile spawner component failed to operate.\n");
			return;
		}
	}
}

void ProjectileSpawner::HandleEvent(Event* pEvent) {}