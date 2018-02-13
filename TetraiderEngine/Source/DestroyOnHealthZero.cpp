#include "GameObject.h"
#include "DestroyOnHealthZero.h"
#include "AOEAttack.h"
#include "Transform.h"
#include "TetraiderAPI.h"

DestroyOnHealthZero::DestroyOnHealthZero(): Component(ComponentType::C_DestroyOnHealthZero), m_destroyIn(0), m_timeStamp(0), m_isDestory(false), m_Attack(nullptr) {}
DestroyOnHealthZero::~DestroyOnHealthZero() {
	if (m_Attack)
		delete m_Attack;
}

void DestroyOnHealthZero::Update(float dt) {
	if (m_isDestory) {
		if (TETRA_FRAMERATE.GetElapsedTime() - m_timeStamp >= m_destroyIn) {
			if (m_isExplode) {
				GameObject* pExplosionGO = TETRA_GAME_OBJECTS.CreateGameObject(m_explosionPrefab);
				pExplosionGO->DestroyIn(3.0f);
				Transform* pExplosion = pExplosionGO->GetComponent<Transform>(C_Transform);
				Transform* pTransform = pGO->GetComponent<Transform>(C_Transform);
				pExplosion->SetPosition(pTransform->GetPosition());
				m_Attack->Use(pTransform->GetPosition());
			}

			pGO->Destroy();
		}
	}

	if (m_isExplode) {
		Transform* pTransform = pGO->GetComponent<Transform>(C_Transform);
		TETRA_DEBUG.DrawWireCircle(pTransform->GetPosition(), m_explosionRadius*2.0f, DebugColor::CYAN);
	}
}

void DestroyOnHealthZero::Serialize(const json& j) {
	m_destroyIn = ParseFloat(j, "destroyIn");
	m_isExplode = ParseBool(j, "isExplode");
	if (m_isExplode) {
		m_explosionRadius = ParseFloat(j, "explosionRadius");
		m_knockBackSpeed = ParseFloat(j, "explosionKnockBack");
		m_explosionDamage = ParseInt(j, "explosionDamage");
		m_explosionPrefab = ParseString(j, "explosionPrefab");
		m_Attack = new AOEAttack(0, m_explosionDamage, m_knockBackSpeed, AttackType::AOE, m_explosionRadius);
	}
}

void DestroyOnHealthZero::LateInitialize() {}

void DestroyOnHealthZero::HandleEvent(Event* pEvent) {
	if (pEvent->Type() == EventType::EVENT_OnHealthZero) {
		m_timeStamp = TETRA_FRAMERATE.GetElapsedTime() - 0.001f;
		m_isDestory = true;
	}
}