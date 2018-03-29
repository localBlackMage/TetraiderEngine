#include <Stdafx.h>

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

void SpawnOnHealthZero::_SpawnPrefab(std::string prefab)
{
	if (prefab == "")	return;
	GameObject* pSpawnedObject = TETRA_GAME_OBJECTS.CreateGameObject(prefab);

	Transform* pTransform = pGO->GetComponent<Transform>(C_Transform);
	Transform* pSpawnedObjectTransform = pSpawnedObject->GetComponent<Transform>(C_Transform);

	pSpawnedObjectTransform->SetPosition(pTransform->GetPosition());
	WaveMovement* pWaveMovement = pSpawnedObject->GetComponent<WaveMovement>(C_WaveMovement);
	if (pWaveMovement) {
		pWaveMovement->ChangeInitialPos(pSpawnedObjectTransform->GetPosition());
	}
}

void SpawnOnHealthZero::_SpawnPrefabs()
{
	switch (m_dropType) {
		case DropType::PREFAB_ONLY: {
			for (std::string prefab : m_prefabs) {
				_SpawnPrefab(prefab);
			}
			break;
		}
		case DropType::RANDOM_ONLY: {
			if (m_randomDrops.size() == 0) return;
			_SpawnPrefab(m_randomDrops[RandomDropIndex()].m_prefabName);
			break;
		}
		case DropType::BOTH: {
			for (std::string prefab : m_prefabs) {
				_SpawnPrefab(prefab);
			}

			if (m_randomDrops.size() == 0) return;
			_SpawnPrefab(m_randomDrops[RandomDropIndex()].m_prefabName);

			break;
		}
	}
}

SpawnOnHealthZero::SpawnOnHealthZero(): Component(ComponentType::C_SpawnOnHealthZero) {}

SpawnOnHealthZero::~SpawnOnHealthZero() {}

void SpawnOnHealthZero::Deactivate() {
	pGO = nullptr; 
}

void SpawnOnHealthZero::Update(float dt) {}

void SpawnOnHealthZero::Serialize(const json& j) {
	std::string dropType = ParseString(j, "dropType");
	if (dropType == "prefab")		m_dropType = DropType::PREFAB_ONLY;
	else if (dropType == "random")	m_dropType = DropType::RANDOM_ONLY;
	else							m_dropType = DropType::BOTH;

	if ((m_dropType == DropType::PREFAB_ONLY || m_dropType == DropType::BOTH) && 
		ValueExists(j, "prefab")) 
	{
		if (j["prefab"].is_array())
			m_prefabs = ParseStringList(j, "prefab");
		else
			m_prefabs.push_back(ParseString(j, "prefab"));
	}
	
	if ((m_dropType == DropType::RANDOM_ONLY || m_dropType == DropType::BOTH) &&
		ValueExists(j, "RandomDrops"))
	{
		int size = j["RandomDrops"].size();
		for (int i = 0; i < size; ++i) {
			m_randomDrops.push_back(RandomDrop(ParseString(j["RandomDrops"][i], "prefab"), ParseInt(j["RandomDrops"][i], "weight")));
		}
	}
}

void SpawnOnHealthZero::LateInitialize() {}

void SpawnOnHealthZero::HandleEvent(Event* pEvent) {
	if (pEvent->Type() == EventType::EVENT_OnHealthZero)
		_SpawnPrefabs();
}

void SpawnOnHealthZero::Override(const json& j) {
	if (ValueExists(j, "spawnOnHealthZero")) {
		if (j["SpawnOnHealthZero"]["prefab"].is_array())
			m_prefabs = ParseStringList(j["SpawnOnHealthZero"], "prefab");
		else
			m_prefabs.push_back(ParseString(j["SpawnOnHealthZero"], "prefab"));
	}
}

void SpawnOnHealthZero::AddSpawnObject(std::string prefab) {
	m_prefabs.push_back( prefab );
}