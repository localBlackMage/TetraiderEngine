//#include "GameObject.h"
//#include "Stamina.h"
//#include "TetraiderAPI.h"

#include <Stdafx.h>

Stamina::Stamina(): Component(ComponentType::C_Stamina) {}
Stamina::~Stamina() {}

void Stamina::Deactivate() {
	pGO = nullptr;
}

void Stamina::Update(float dt) {
	if (TETRA_GAME_STATE.IsGamePaused()) return;

	if (!m_isFull) {
		m_timeFromLastUsed += dt;
		if (m_timeFromLastUsed > m_rechargeCoolDown) {
			m_isEmpty = false;
			m_currentStamina += m_rechargeSpeed*dt;
			if (m_currentStamina > m_maxStamina) {
				m_currentStamina = m_maxStamina;
				m_isFull = true;
			}

			TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EventType::EVENT_StaminaRecharge, &StaminaChangeData(m_currentStamina, m_maxStamina)));
		}
	}
}

void Stamina::Serialize(const json& j) {
	m_currentStamina = ParseFloat(j, "currentStamina");
	m_maxStamina = ParseFloat(j, "maxStamina");
	m_useSpeed = ParseFloat(j, "useSpeed");
	m_rechargeSpeed = ParseFloat(j, "rechargeSpeed");
	m_rechargeCoolDown = ParseFloat(j, "rechargeCoolDown");

	m_isFull = (m_currentStamina == m_maxStamina);
	m_isEmpty = (m_currentStamina == 0);
}

void Stamina::LateInitialize() {}

void Stamina::HandleEvent(Event* pEvent) {}

bool Stamina::UseStamina(float dt) {
	m_timeFromLastUsed = 0;
	if (m_isEmpty) {
		return false;
	}

	m_isFull = false;
	m_currentStamina -= m_useSpeed*dt;
	if (m_currentStamina < 0) {
		m_currentStamina = 0;
		m_isEmpty = true;
	}

	TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EventType::EVENT_StaminaUse, &StaminaChangeData(m_currentStamina, m_maxStamina)));

	return true;
}