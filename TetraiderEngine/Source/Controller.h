#pragma once
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Agent.h"

class Weapon;

class Controller : public Agent {
public:
	Controller();
	~Controller();
	static Component* CreateInstance() { return new Controller(); }
	virtual void DeActivate();
	virtual void Update(float dt);
	virtual void LateInitialize();
	virtual void Serialize(const json& j);
	virtual void HandleEvent(Event* pEvent);
protected:
	Weapon* m_pWeapon;
private:
	bool m_isGameControllerEnabled;
	void CheckToggleMouseControl();
	float m_flySpeed;
};

#endif

