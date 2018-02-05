#include "MeleeAttack.h"
#include "Weapon.h"
#include "TetraiderAPI.h"
#include "Transform.h"
#include "Math\Collisions.h"
#include "GameObject.h"
#include "Health.h"
#include "Controller.h"
#include "Animation.h"
#include "Audio.h"

MeleeAttack::MeleeAttack(float coolDown, int baseDamage, AttackType type, float radius, float angle, float triggerAttackIn):
	Attack(coolDown, baseDamage, type),
	m_radius(radius),
	m_angle(angle),
	m_triggerAttackIn(triggerAttackIn) {
	// Dummy code
	m_pSlashEffect = TETRA_GAME_OBJECTS.CreateGameObject("P_SlashTest");
	//-----------
}

MeleeAttack::~MeleeAttack() {
	m_pSlashEffect->Destroy();
}

void MeleeAttack::Update(float dt) {
	Attack::Update(dt);

	if (m_isAttacking) Run();

	// Dummy code
	Transform* pOwnerTransform = m_pOwner->pGO->GetComponent<Transform>(ComponentType::C_Transform);
	Controller* pOwnerController = m_pOwner->pGO->GetComponent<Controller>(ComponentType::C_Controller);
	Transform* pSlashEffectTransform = m_pSlashEffect->GetComponent<Transform>(ComponentType::C_Transform);
	if (pOwnerTransform && pOwnerController && pSlashEffectTransform) {
		pSlashEffectTransform->SetPosition(pOwnerTransform->GetPosition() + pOwnerController->GetLookDirection() * 75);
		pSlashEffectTransform->SetAngleZ(atan2f(pOwnerController->GetLookDirection().y, pOwnerController->GetLookDirection().x) * 180 / PI);
	}
	//------------
}

// Assumes direction is normalized
bool MeleeAttack::Use(const Vector3D& direction) {
	if (!Attack::Use(direction)) return false;

	m_dirToAttackIn = direction;
	Run();
	// Dummy code
	Animation* pAnimation = m_pSlashEffect->GetComponent<Animation>(ComponentType::C_Animation);
	if(pAnimation)
		pAnimation->Play(0);

	Audio* pAudio = m_pSlashEffect->GetComponent<Audio>(ComponentType::C_Audio);
	if (pAudio)
		pAudio->Play();
	//---------------
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

void MeleeAttack::Debug(const Vector3D& dir) {
	Transform* pTransform = m_pOwner->pGO->GetComponent<Transform>(ComponentType::C_Transform);
	TETRA_DEBUG.DrawWireCone(pTransform->GetPosition(), Vector3D(0.0f, 0.0f, atan2f(dir.y, dir.x)*180/PI), m_angle, m_radius, DebugColor::YELLOW);
}
