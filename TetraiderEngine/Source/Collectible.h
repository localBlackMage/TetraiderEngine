#pragma once
#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

//#include "Component.h"
//#include "Math\Vector3D.h"

class Collectible : public Component {
public:
	Collectible();
	~Collectible();
	static Component* CreateInstance() { return new Collectible(); }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void Serialize(const json& j);
	virtual void LateInitialize();
	virtual void LateUpdate(float dt) {}
	virtual void HandleEvent(Event* pEvent);
private:
	bool m_isHealthPickUp;
	bool m_isEgg;
	bool m_isAmmo;
	int m_value;
};

#endif

