#pragma once
#ifndef SPAWN_ONHEALTH_ZERO_H
#define SPAWN_ONHEALTH_ZERO_H

//#include "Component.h"
//#include "Math/MathLibs.h"
//
//class Transform;

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
private:
	std::string m_prefab;
};

#endif

