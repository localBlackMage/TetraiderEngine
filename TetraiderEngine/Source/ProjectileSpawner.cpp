#include "GameObject.h"
#include "Weapon.h"
#include "ProjectileSpawner.h"
#include "TetraiderAPI.h"

ProjectileSpawner::ProjectileSpawner(): Component(ComponentType::C_ProjectileSpawner), m_diriection(Vector3D()), m_isActive(true) {}
ProjectileSpawner::~ProjectileSpawner() {}

void ProjectileSpawner::DeActivate() {
}

void ProjectileSpawner::Update(float dt) {
}

void ProjectileSpawner::Serialize(const json& j) {
	m_isActive = ParseBool(j, "isActive");
	m_diriection.x = ParseFloat(j["direction"], "x");
	m_diriection.y = ParseFloat(j["direction"], "y");
	m_diriection.z = ParseFloat(j["direction"], "z");
}

void ProjectileSpawner::LateInitialize() {}

void ProjectileSpawner::HandleEvent(Event* pEvent) {}