/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

Collectible::Collectible(): Component(ComponentType::C_Collectible) {}
Collectible::~Collectible() {}

void Collectible::Deactivate() {
	pGO = nullptr; 
}

void Collectible::Update(float dt) {}

void Collectible::Serialize(const json& j) {
	m_isHealthPickUp = ParseBool(j, "isHealth");
	m_isEgg = ParseBool(j, "isEgg");
	m_isAmmo = ParseBool(j, "isAmmo");
	m_isGoldenFeather = ParseInt(j, "isGoldenFeather");
	m_value = ParseInt(j, "value");
}

void Collectible::LateInitialize() {}

void Collectible::HandleEvent(Event* pEvent) {
	bool isCollected = false;
	switch (pEvent->Type()) {
		case EVENT_OnCollide: {
			OnCollideData* pData = pEvent->Data<OnCollideData>();
			if (pData->pGO->m_tag != T_Player) return;

			if (m_isEgg) {
				TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_EggCollected, &CollectibleData(m_value)));
				isCollected = true;
				pGO->Destroy();
			}
			else if (m_isHealthPickUp) {
				Health* pHealth = pData->pGO->GetComponent<Health>(C_Health);
				if (!pHealth->IsHealthFull()) {
					int extraHealth = 0;
					TETRA_PLAYERSTATS.IsPowerUpActive(PowerUpType::IncreaseHealthFromPickUp, extraHealth);
					pHealth->HandleEvent(&Event(EVENT_HealthCollected, &CollectibleData(m_value + extraHealth)));
					isCollected = true;
					pGO->Destroy();
				}
			}
			else if (m_isAmmo) {
				int extraArrows = 0;
				TETRA_PLAYERSTATS.IsPowerUpActive(PowerUpType::IncreaseAmmoPickUp, extraArrows);
				TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_AmmoUpdate, &CollectibleData(m_value + extraArrows)));
				isCollected = true;
				pGO->Destroy();
			}
			else if (m_isGoldenFeather) {
				TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_GoldenFeatherCollected, &CollectibleData(m_value)));
				isCollected = true;
				pGO->Destroy();
			}

			if (isCollected) {
				Audio* pAudio = pGO->GetComponent<Audio>(C_Audio);
				if (pAudio)
					pAudio->Play();
			}

			break;
		}
	}
}