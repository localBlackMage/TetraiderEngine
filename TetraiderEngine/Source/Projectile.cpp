//#include "Projectile.h"
//#include "Transform.h"
//#include "Body.h"
//#include "GameObject.h"
//#include "Event.h"
//#include "Health.h"
//#include "TetraiderAPI.h"
//#include "Audio.h"

#include <Stdafx.h>

Projectile::Projectile() : Component(C_Projectile), m_currentLifeTime(0) {}

void Projectile::Update(float dt) {
	if (TETRA_GAME_STATE.IsGamePaused()) return;

	//m_pBody->SetVelocity(Lerp(m_pBody->Get))
	if(m_isAccelerateToZero)
		m_pBody->SetVelocity(Lerp(m_pBody->GetVelocity(), Vector3D(), dt*m_deccelerationSpeed));

	if (m_isRotate) {
		m_pTransform->RotateZ(-m_pBody->GetVelocity().Length() * dt);
	}

	if (m_isExplodeOnVelocityZero) {
		if (m_pBody->GetVelocity().IsVectorZero(10.0f)) {
			pGO->HandleEvent(&Event(EVENT_Explode));
		}
	}

	m_currentLifeTime += dt;
	if (m_currentLifeTime > m_lifeTime && !m_isExplodeOnVelocityZero)
		pGO->Destroy();
}

void Projectile::Deactivate() { 
	pGO = nullptr; 
	m_pBody = nullptr; 
	m_pTransform = nullptr; 
}

void Projectile::Serialize(const json& j) {
	m_isAccelerateToZero = ParseBool(j , "isAccelerateToZero");
	m_isRotate = ParseBool(j, "isRotate");
	m_isExplodeOnVelocityZero = ParseBool(j, "isExplodeOnVelocityZero");
	m_isIgnoreCollideEvent = ParseBool(j, "isIgnoreCollideEvent");
	m_deccelerationSpeed = ParseFloat(j, "deacclerationSpeed");
}

void Projectile::HandleEvent(Event* pEvent) {
	if (pEvent->Type() == EVENT_OnCollide) {
		OnCollideData* collisionData = pEvent->Data<OnCollideData>();

		if (!m_isIgnoreCollideEvent) {
			if (!collisionData->pGO) return;

			// Avoid friendly fire
			if (m_projectileType == ProjectileType::EnemyProjectile && collisionData->pGO->m_tag == T_Enemy) return;
			else if (m_projectileType == ProjectileType::PlayerProjectile && collisionData->pGO->m_tag == T_Player) return;
			else if (collisionData->pGO->m_tag == T_Projectile) return;
			else if (collisionData->pGO == m_pOwner) return;
			else if (collisionData->pGO->m_tag == T_Hazard || collisionData->pGO->m_tag == T_None || collisionData->pGO->m_tag == T_DeadEnemy) return;

			// If object has health component, deal damage before destroying itself
			Health* pHealth = collisionData->pGO->GetComponent<Health>(ComponentType::C_Health);
			Transform* pTrans = collisionData->pGO->GetComponent<Transform>(ComponentType::C_Transform);
			Vector3D dirOfAttack = pTrans->GetPosition() - m_pTransform->GetPosition();
			dirOfAttack.Normalize();
			if (pHealth) pHealth->TakeDamage(m_damage, dirOfAttack, m_knockBackSpeed, false);

			pGO->Destroy();
		}
		else {
			m_pTransform->SetPosition(m_pTransform->GetPosition() + collisionData->mtv.normal*collisionData->mtv.penetration);
		}
	}
}

void Projectile::SetProperties(int damage, float speed, const Vector3D& dir, float lifeTime, bool isEnemyProjectile, float knockBackSpeed, GameObject* owner) {
	m_pTransform->SetAngleZ(atan2f(dir.y, dir.x)*180/PI);
	m_damage = damage;
	m_lifeTime = lifeTime;
	if (isEnemyProjectile)
		m_projectileType = ProjectileType::EnemyProjectile;
	else
		m_projectileType = ProjectileType::PlayerProjectile;
	m_pBody->SetVelocity(speed*dir);
	m_knockBackSpeed = knockBackSpeed;
	m_pOwner = owner;

	Audio* m_pAudio = pGO->GetComponent<Audio>(C_Audio);
	if (m_pAudio)
		m_pAudio->Play();
}

void Projectile::SetProperties(int damage, float speed, const Vector3D& dir, float lifeTime, float knockBackSpeed, GameObject* owner) {
	m_pTransform->SetAngleZ(atan2f(dir.y, dir.x) * 180 / PI);
	m_damage = damage;
	m_lifeTime = lifeTime;
	m_projectileType = ProjectileType::Neutral;
	m_pBody->SetVelocity(speed*dir);
	m_knockBackSpeed = knockBackSpeed;
	m_pOwner = owner;

	Audio* m_pAudio = pGO->GetComponent<Audio>(C_Audio);
	if (m_pAudio)
		m_pAudio->Play();
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