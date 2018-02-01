#include "MeleeAttack.h"
#include "TetraiderAPI.h"

void MeleeAttack::Update(float dt) {
	Attack::Update(dt);

	if (m_isAttacking)
		Attack(dt);
}

// Assumes direction is normalized
bool MeleeAttack::Use(const Vector3D& direction) {
	if (m_isOnCooldown)
		return false;

	dirToAttackIn = direction;
	return Attack::Use(direction);
}

void MeleeAttack::Attack(float dt) {
	if (T_FRAMERATE.GetElapsedTime() - m_lastUsedTimeStamp < m_triggerAttackIn) return;

	// Do cone calculation here with radius direction and angle
	m_isAttacking = false;
}
