#pragma once
#ifndef SPAWN_ONHEALTH_ZERO_H
#define SPAWN_ONHEALTH_ZERO_H

//#include "Component.h"
//#include "Math/MathLibs.h"
//
//class Transform;

struct RandomDrop {
	RandomDrop(std::string name, int weight) : m_prefabName(name), m_weight(weight) {}
	std::string m_prefabName;
	int m_weight;
};

class SpawnOnHealthZero : public Component {
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
	void SetSpawnObject(std::string prefab);
private:
	std::string m_prefab;
	bool m_isRandom;
	std::vector<RandomDrop> m_randomDrops;
	int RandomDropIndex();
};

#endif

