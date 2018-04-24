/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

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

		if (m_isWeaponFlash && m_timeFromLastUsed > m_coolDown / 2.0f) {
			m_pOwner->HideWeapon(false);
			m_isWeaponFlash = false;
		}
	}
}

void Attack::UpdateCoolDown(float multiplier) {
	m_coolDown = m_coolDown / multiplier;
}

void Attack::MultiplyDamage(float multiplier) {
	m_baseDamage = (int)(m_baseDamage*multiplier);
}