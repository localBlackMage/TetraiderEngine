#pragma once
#ifndef SPAWN_ONHEALTH_ZERO_H
#define SPAWN_ONHEALTH_ZERO_H

struct RandomDrop {
	RandomDrop(std::string name, int weight) : m_prefabName(name), m_weight(weight) {}
	std::string m_prefabName;
	int m_weight;
};

enum DropType {
	PREFAB_ONLY = 0,
	RANDOM_ONLY,
	BOTH
};

class SpawnOnHealthZero : public Component {
private:
	
	std::vector<std::string> m_prefabs;
	DropType m_dropType;
	std::vector<RandomDrop> m_randomDrops;

	int RandomDropIndex();
	// For a given prefab name, will call the GOM to spawn it and set it's position accordingly
	void _SpawnPrefab(std::string prefab);

	// Will spawn either predetermined prefabs, random prefabs, or both depending on this
	// component's DropType
	void _SpawnPrefabs();
public:
	SpawnOnHealthZero();
	~SpawnOnHealthZero();
	static Component* CreateInstance() { return new SpawnOnHealthZero(); }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt) {}
	virtual void HandleEvent(Event* pEvent);
	virtual void Override(const json& j);

	// Adds the new prefab to the prefab list
	void AddSpawnObject(std::string prefab);

	// Sets this component to only spawn prefabs within the prefab list, if any
	inline void DisableRandom() { m_dropType = DropType::PREFAB_ONLY; }

	// Sets this component to only spawn random prefabs from within the randomDrops list, if any
	inline void EnableOnlyRandom() { m_dropType = DropType::RANDOM_ONLY; }

	// Enables this component to spawn both predetermined prefabs within the prefab list as
	// well as random prefabs from within the randomDrops list, if any
	inline void EnableRandomAndPrefab() { m_dropType = DropType::BOTH; }
};

#endif

