#include <Stdafx.h>

SpawnOnHealthZero::SpawnOnHealthZero(): Component(ComponentType::C_SpawnOnHealthZero) {}
SpawnOnHealthZero::~SpawnOnHealthZero() {}

void SpawnOnHealthZero::Deactivate() {
	pGO = nullptr; 
}

void SpawnOnHealthZero::Update(float dt) {}

void SpawnOnHealthZero::Serialize(const json& j) {
	m_prefab = ParseString(j, "prefab");
	m_isRandom = ParseBool(j, "isRandom");

	if (m_isRandom) {
		int size = j["RandomDrops"].size();
		for (int i = 0; i < size; ++i) {
			m_randomDrops.push_back(RandomDrop(ParseString(j["RandomDrops"][i], "prefab"), ParseInt(j["RandomDrops"][i], "weight")));
		}
	}
}

void SpawnOnHealthZero::LateInitialize() {}

void SpawnOnHealthZero::HandleEvent(Event* pEvent) {
	if (pEvent->Type() == EventType::EVENT_OnHealthZero) {
		GameObject* pSpawnedObject;
		if (m_isRandom) {
			if (m_randomDrops.size() == 0) return;

			int index = RandomDropIndex();
			if (m_randomDrops[index].m_prefabName == "") return;

			pSpawnedObject = TETRA_GAME_OBJECTS.CreateGameObject(m_randomDrops[index].m_prefabName);
		}
		else {
			pSpawnedObject = TETRA_GAME_OBJECTS.CreateGameObject(m_prefab);
		}

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

int SpawnOnHealthZero::RandomDropIndex() {
	float totalWeight = 0;
	float randomNumber = RandomFloat(0, 1);
	for (unsigned int i = 0; i < m_randomDrops.size(); ++i) {
		totalWeight += (float)m_randomDrops[i].m_weight;
	}

	float lastProbability = 0;
	for (unsigned int i = 0; i < m_randomDrops.size(); ++i) {
		float probability = (float)m_randomDrops[i].m_weight / totalWeight + lastProbability;
		if (randomNumber < probability)
			return i;
		else
			lastProbability += probability;
	}

	return 0;
}

void SpawnOnHealthZero::SetSpawnObject(std::string prefab) {
	m_isRandom = false;
	m_prefab = prefab;
}