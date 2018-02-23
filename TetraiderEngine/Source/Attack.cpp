#include "Attack.h"
#include "TetraiderAPI.h"

// Assumes direction is normalized
bool Attack::Use(const Vector3D& direction) {
	if (m_isOnCooldown) return false;

	m_timeFromLastUsed = 0;
	m_isOnCooldown = true;
	m_isAttacking = true;
	return true;
}

void Attack::Update(float dt) {
	if (TETRA_GAME_STATE.IsGamePaused()) return; 

	if (m_isOnCooldown) {
		m_timeFromLastUsed += dt;
		if (m_timeFromLastUsed > m_coolDown)
			m_isOnCooldown = false;
	}
}