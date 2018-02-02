#include "MeleeAttack.h"
#include "TetraiderAPI.h"

void MeleeAttack::Update(float dt) {
	Attack::Update(dt);

	if (m_isAttacking) Attack();
}

// Assumes direction is normalized
bool MeleeAttack::Use(const Vector3D& direction) {
	if (!Attack::Use(direction)) return false;

	dirToAttackIn = direction;
	Attack();
	return true;
}

void MeleeAttack::Attack() {
	if (T_FRAMERATE.GetElapsedTime() - m_lastUsedTimeStamp < m_triggerAttackIn) return;

	// Do cone calculation here with radius direction and angle on health components
	m_isAttacking = false;
}
