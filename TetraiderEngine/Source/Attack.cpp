#include "Attack.h"
#include "TetraiderAPI.h"

// Assumes direction is normalized
bool Attack::Use(const Vector3D& direction) {
	m_lastUsedTimeStamp = T_FRAMERATE.GetElapsedTime();
	m_isOnCooldown = true;
	m_isAttacking = true;
	return true;
}

void Attack::Update(float dt) {
	if (m_isOnCooldown) {
		if (T_FRAMERATE.GetElapsedTime() - m_lastUsedTimeStamp > m_coolDown)
			m_isOnCooldown = false;
	}
}