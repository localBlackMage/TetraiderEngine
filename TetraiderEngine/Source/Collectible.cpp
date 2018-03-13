//#include "GameObject.h"
//#include "Collectible.h"
//#include "Health.h"
//#include "TetraiderAPI.h"

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
	switch (pEvent->Type()) {
		case EVENT_OnCollide: {
			OnCollideData* pData = pEvent->Data<OnCollideData>();
			if (pData->pGO->m_tag != T_Player) return;

			if (m_isEgg) {
				TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_EggCollected, &CollectibleData(m_value)));
				pGO->Destroy();
			}
			else if (m_isHealthPickUp) {
				Health* pHealth = pData->pGO->GetComponent<Health>(C_Health);
				if (!pHealth->IsHealthFull()) {
					pHealth->HandleEvent(&Event(EVENT_HealthCollected, &CollectibleData(m_value)));
					pGO->Destroy();
				}
			}
			else if (m_isAmmo) {
				int extraArrows = TETRA_PLAYERSTATS.GetAmmoPickUpUpgrade();
				TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_AmmoUpdate, &CollectibleData(m_value + extraArrows)));
				pGO->Destroy();
			}
			else if (m_isGoldenFeather) {
				TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_GoldenFeatherCollected, &CollectibleData(m_value)));
				pGO->Destroy();
			}

			break;
		}
	}
}