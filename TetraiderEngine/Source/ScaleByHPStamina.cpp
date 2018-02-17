#include "GameObject.h"
#include "Transform.h"
#include "ScaleByHPStamina.h"
#include "TetraiderAPI.h"
#include "Health.h"
#include "Stamina.h"

ScaleByHPStamina::ScaleByHPStamina(): Component(ComponentType::C_ScaleByHPStamina), m_originalScale(0), m_isScaleByHealth(true) {}
ScaleByHPStamina::~ScaleByHPStamina() {}

void ScaleByHPStamina::DeActivate() {
	pGO = nullptr;
	m_pTransform = nullptr;
}

void ScaleByHPStamina::Update(float dt) {}

void ScaleByHPStamina::Serialize(const json& j) {
	m_isScaleByHealth = ParseBool(j, "isScaleByHealth");
}

void ScaleByHPStamina::LateInitialize() {
	if (!m_pTransform) {
		if (pGO)
			m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
		else {
			printf("No Game Object found. ScaleByHPStamina component failed to operate.\n");
			return;
		}

		if (!m_pTransform) {
			printf("No Transform component found. ScaleByHPStamina component failed to operate.\n");
			assert(m_pTransform);
			return;
		}
	}

	m_originalScale = m_pTransform->GetScaleX();

	if (m_isScaleByHealth) {
		TETRA_EVENTS.Subscribe(EventType::EVENT_OnTakeDamage, this);
		TETRA_EVENTS.Subscribe(EventType::EVENT_OnPlayerHeal, this);
	}
	else {
		TETRA_EVENTS.Subscribe(EventType::EVENT_StaminaUse, this);
		TETRA_EVENTS.Subscribe(EventType::EVENT_StaminaRecharge, this);
	}
	
	TETRA_EVENTS.Subscribe(EventType::EVENT_OnLevelInitialized, this);
}

void ScaleByHPStamina::HandleEvent(Event* pEvent) {
	switch (pEvent->Type()) {
		case EventType::EVENT_OnTakeDamage: {
			if (m_isScaleByHealth) {
				HealthChangeData* healthData = pEvent->Data<HealthChangeData>();
				m_pTransform->SetScaleX(((float)healthData->mCurrentHealth / (float)healthData->mMaxHealth)*m_originalScale);
			}
			break;
		}
		case EventType::EVENT_OnPlayerHeal : {
			if (m_isScaleByHealth) {
				HealthChangeData* healthData = pEvent->Data<HealthChangeData>();
				m_pTransform->SetScaleX(((float)healthData->mCurrentHealth / (float)healthData->mMaxHealth)*m_originalScale);
			}
			break;
		}
		case EventType::EVENT_OnLevelInitialized: {
			if (m_isScaleByHealth) {
				const GameObject* pPlayer = TETRA_GAME_OBJECTS.GetPlayer();
				const Health* pHealth = pPlayer->GetComponent<Health>(C_Health);
				m_pTransform->SetScaleX(((float)pHealth->GetHealth() / (float)pHealth->GetMaxHealth())*m_originalScale);
			}
			else {
				const GameObject* pPlayer = TETRA_GAME_OBJECTS.GetPlayer();
				const Stamina* pStamina = pPlayer->GetComponent<Stamina>(C_Stamina);
				m_pTransform->SetScaleX((pStamina->GetCurrentStamina() / pStamina->GetMaxStamina())*m_originalScale);
			}
			break;
		}
		case EventType::EVENT_StaminaUse: {
			if (!m_isScaleByHealth) {
				StaminaChangeData* staminaData = pEvent->Data<StaminaChangeData>();
				m_pTransform->SetScaleX((staminaData->mCurrentStamina / staminaData->mMaxStamina)*m_originalScale);
			}
			break;
		}
		case EventType::EVENT_StaminaRecharge: {
			if (!m_isScaleByHealth) {
				StaminaChangeData* staminaData = pEvent->Data<StaminaChangeData>();
				m_pTransform->SetScaleX((staminaData->mCurrentStamina / staminaData->mMaxStamina)*m_originalScale);
			}
			break;
		}
	}
}