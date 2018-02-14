#include "Projectile.h"
#include "Transform.h"
#include "Body.h"
#include "GameObject.h"
#include "Event.h"
#include "Health.h"
#include "TetraiderAPI.h"

Projectile::Projectile() : Component(C_Projectile) {}

void Projectile::Update(float dt) {
	if (TETRA_FRAMERATE.GetElapsedTime() - m_creationLifeTime > m_lifeTime)
		pGO->Destroy();
}

void Projectile::DeActivate() { 
	pGO = nullptr; 
	m_pBody = nullptr; 
	m_pTransform = nullptr; 
}

void Projectile::Serialize(const json& j) {}

void Projectile::HandleEvent(Event* pEvent) {
	if (pEvent->Type() == EVENT_OnCollide) {
		OnCollideData* collisionData = pEvent->Data<OnCollideData>();

		// Avoid friendly fire
		if (m_isEnemyProjectile && collisionData->pGO->m_tag == T_Enemy) return;
		else if (!m_isEnemyProjectile && collisionData->pGO->m_tag == T_Player) return;
		else if (!m_isEnemyProjectile && collisionData->pGO->m_tag == T_Projectile) return;
		else if (collisionData->pGO->m_tag == T_Hazard) return;

		// If object has health component, deal damage before destroying itself
		Health* pHealth = collisionData->pGO->GetComponent<Health>(ComponentType::C_Health);
		Transform* pTrans = collisionData->pGO->GetComponent<Transform>(ComponentType::C_Transform);
		Vector3D dirOfAttack = pTrans->GetPosition() - m_pTransform->GetPosition();
		dirOfAttack.Normalize();
		if (pHealth) pHealth->TakeDamage(m_damage, dirOfAttack, m_knockBackSpeed);

		pGO->Destroy();
	}
}

void Projectile::SetProperties(const Vector3D& position, int damage, float speed, const Vector3D& dir, float lifeTime, bool isEnemyProjectile, float knockBackSpeed) {
	m_pTransform->SetPosition(position);
	m_pTransform->SetAngleZ(atan2f(dir.y, dir.x)*180/PI);
	m_damage = damage;
	m_lifeTime = lifeTime;
	m_isEnemyProjectile = isEnemyProjectile;
	m_pBody->SetVelocity(speed*dir);
	m_knockBackSpeed = knockBackSpeed;
	m_creationLifeTime = TETRA_FRAMERATE.GetElapsedTime();
}

void Projectile::LateInitialize() {
	if (!m_pTransform) {
		if (pGO)
			m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
		else {
			printf("No Game Object found. Projectile component failed to operate.\n");
			return;
		}

		if (!m_pTransform) {
			printf("No Transform component found. Projectile component failed to operate.\n");
			assert(m_pTransform);
			return;
		}
	}

	if (!m_pBody) {
		if (pGO)
			m_pBody = pGO->GetComponent<Body>(ComponentType::C_Body);
		else {
			printf("No Game Object found. Projectile component failed to operate.\n");
			return;
		}

		if (!m_pBody) {
			printf("No Body component found. Projectile component failed to operate.\n");
			assert(m_pBody);
			return;
		}
	}
}