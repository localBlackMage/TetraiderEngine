#include "MeleeAttack.h"
#include "Weapon.h"
#include "TetraiderAPI.h"
#include "TetraiderAPI.h"
#include "Transform.h"
#include "Math\Collisions.h"
#include "GameObject.h"
#include "Health.h"

MeleeAttack::MeleeAttack(float coolDown, int baseDamage, float radius, float angle, float triggerAttackIn): 
	Attack(coolDown, baseDamage),
	m_radius(radius),
	m_angle(angle),
	m_triggerAttackIn(triggerAttackIn)
	{}

void MeleeAttack::Update(float dt) {
	Attack::Update(dt);

	if (m_isAttacking) Run();
}

// Assumes direction is normalized
bool MeleeAttack::Use(const Vector3D& direction) {
	if (!Attack::Use(direction)) return false;

	m_dirToAttackIn = direction;
	Run();
	return true;
}

void MeleeAttack::Run() {
	if (TETRA_FRAMERATE.GetElapsedTime() - m_lastUsedTimeStamp < m_triggerAttackIn) return;

	// If enemy, just check for the player/enviromental objects
	GameObjectTag ignoreTag = T_None;
	if (m_pOwner->pGO->m_tag == T_Enemy) {
		ignoreTag = T_Enemy;

	}
	// If player, just check for enemies/enviromental objects
	else if(m_pOwner->pGO->m_tag == T_Player) {
		ignoreTag = T_Player;
	}

	Transform* myOwnerTransform = m_pOwner->pGO->GetComponent<Transform>(ComponentType::C_Transform);
	Vector3D sourceOfAttack = myOwnerTransform->GetPosition();

	for (unsigned int i = 0; i < TETRA_GAME_OBJECTS.mGameObjectsWithHealthComponents.size(); ++i) {
		if (TETRA_GAME_OBJECTS.mGameObjectsWithHealthComponents[i]->m_tag == ignoreTag)
			continue;

		Transform* pTransform = TETRA_GAME_OBJECTS.mGameObjectsWithHealthComponents[i]->GetComponent<Transform>(ComponentType::C_Transform);

		if (IsPointInCone(pTransform->GetPosition(), sourceOfAttack, m_radius, m_dirToAttackIn, m_angle)) {
			Health* pHealth = TETRA_GAME_OBJECTS.mGameObjectsWithHealthComponents[i]->GetComponent<Health>(ComponentType::C_Health);
			// TODO: Modify attack damage to take character stats in consideration
			pHealth->TakeDamage(m_baseDamage, sourceOfAttack);
		}
	}

	m_isAttacking = false;
}
