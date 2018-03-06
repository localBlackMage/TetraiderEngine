//#include "GameObject.h"
//#include "SpawnOnHealthZero.h"
//#include "Transform.h"
//#include "TetraiderAPI.h"
//#include "WaveMovement.h"

#include <Stdafx.h>

SpawnOnHealthZero::SpawnOnHealthZero(): Component(ComponentType::C_SpawnOnHealthZero) {}
SpawnOnHealthZero::~SpawnOnHealthZero() {}

void SpawnOnHealthZero::Deactivate() {
	pGO = nullptr; 
}

void SpawnOnHealthZero::Update(float dt) {}

void SpawnOnHealthZero::Serialize(const json& j) {
	m_prefab = ParseString(j, "prefab");
}

void SpawnOnHealthZero::LateInitialize() {}

void SpawnOnHealthZero::HandleEvent(Event* pEvent) {
	if (pEvent->Type() == EventType::EVENT_OnHealthZero) {
		GameObject* pSpawnedObject = TETRA_GAME_OBJECTS.CreateGameObject(m_prefab);
		Transform* pTransform = pGO->GetComponent<Transform>(C_Transform);
		Transform* pSpawnedObjectTransform = pSpawnedObject->GetComponent<Transform>(C_Transform);

		pSpawnedObjectTransform->SetPosition(pTransform->GetPosition());
		WaveMovement* pWaveMovement = pSpawnedObject->GetComponent<WaveMovement>(C_WaveMovement);
		if (pWaveMovement) {
			pWaveMovement->ChangeInitialPos(pSpawnedObjectTransform->GetPosition());
		}
	}
}

void SpawnOnHealthZero::Override(const json& j) {
	if (ValueExists(j, "spawnOnHealthZero")) {
		m_prefab = ParseString(j["SpawnOnHealthZero"], "prefab");
	}
}