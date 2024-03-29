/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

DestroyOnHealthZero::DestroyOnHealthZero(): Component(ComponentType::C_DestroyOnHealthZero), m_destroyIn(0), m_timeStamp(0), m_isDestory(false), m_Attack(nullptr) {}
DestroyOnHealthZero::~DestroyOnHealthZero() {
	if (m_Attack)
		delete m_Attack;
}

void DestroyOnHealthZero::Deactivate() {
	pGO = nullptr; 
	if(m_Attack)
		delete m_Attack; 

	m_Attack = nullptr;
	m_destroyIn = 0;  
	m_timeStamp = 0, 
	m_isDestory = false; 
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
			if (m_isSpawnAcidPool) {
				Transform* pTransform = pGO->GetComponent<Transform>(C_Transform);
				TETRA_GAME_OBJECTS.CreateGameObject(m_acidPrefab, true, pTransform->GetPosition());
			}

			if (pGO->m_tag != T_Enemy) {
				pGO->Destroy();
			}
			else {
				m_isDestory = false;
			}

			Audio* pAudio = pGO->GetComponent<Audio>(C_Audio);
			if (pAudio)
				pAudio->Play();
		}
	}

	if (m_isExplode) {
		Transform* pTransform = pGO->GetComponent<Transform>(C_Transform);
		TETRA_DEBUG.DrawWireCircle(pTransform->GetPosition(), m_explosionRadius*2.0f, DebugColor::CYAN);
	}
}

void DestroyOnHealthZero::Serialize(const json& j) {
	m_destroyIn = ParseFloat(j, "destroyIn");
	m_isSpawnAcidPool = ParseBool(j, "isSpawnAcidPool");
	if (m_isSpawnAcidPool) {
		m_acidPrefab = ParseString(j, "acidPrefab");
	}
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
	if (pEvent->Type() == EventType::EVENT_OnHealthZero || pEvent->Type() == EventType::EVENT_Explode) {
		m_timeStamp = TETRA_FRAMERATE.GetElapsedTime() - 0.001f;
		m_isDestory = true;
	}
}