#include "MeleeAttack.h"
#include "Weapon.h"
#include "TetraiderAPI.h"
#include "Transform.h"
#include "Math\Collisions.h"
#include "GameObject.h"
#include "Health.h"

MeleeAttack::MeleeAttack(float coolDown, int baseDamage, float knockBackSpeed, AttackType type, float radius, float angle, float triggerAttackIn):
	Attack(coolDown, baseDamage, knockBackSpeed, type),
	m_radius(radius),
	m_angle(angle),
	m_triggerAttackIn(triggerAttackIn) {}

MeleeAttack::~MeleeAttack() {}

void MeleeAttack::Update(float dt) {
	Attack::Update(dt);

	if (m_isAttacking) Run();
}

// Assumes direction is normalized
bool MeleeAttack::Use(const Vector3D& direction) {
	if (!Attack::Use(direction)) return false;

	m_dirToAttackIn = direction;
	Run();
	m_pOwner->PlayEffect();
	return true;
}

void MeleeAttack::Run() {
	if (m_timeFromLastUsed < m_triggerAttackIn) return;

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

	const std::vector<GameObject*> gameObjectsWithHealthComponents = TETRA_GAME_OBJECTS.GetObjectsWithHealthComponents();

	for (unsigned int i = 0; i < gameObjectsWithHealthComponents.size(); ++i) {
		if (gameObjectsWithHealthComponents[i]->m_tag == ignoreTag)
			continue;

		Transform* pTransform = gameObjectsWithHealthComponents[i]->GetComponent<Transform>(ComponentType::C_Transform);

		if (IsPointInCone(pTransform->GetPosition(), sourceOfAttack, m_radius, m_dirToAttackIn, m_angle)) {
			Health* pHealth = gameObjectsWithHealthComponents[i]->GetComponent<Health>(ComponentType::C_Health);
			// TODO: Modify attack damage to take character stats in consideration
			Vector3D dirOfAttack = pTransform->GetPosition() - sourceOfAttack;
			dirOfAttack.Normalize();
			pHealth->TakeDamage(m_baseDamage, dirOfAttack, m_knockBackSpeed);
		}
	}

	m_isAttacking = false;
}

void MeleeAttack::Debug(const Vector3D& dir) {
	Transform* pTransform = m_pOwner->pGO->GetComponent<Transform>(ComponentType::C_Transform);
	TETRA_DEBUG.DrawWireCone(pTransform->GetPosition(), Vector3D(0.0f, 0.0f, atan2f(dir.y, dir.x)*180/PI), m_angle, m_radius, DebugColor::YELLOW);
}
