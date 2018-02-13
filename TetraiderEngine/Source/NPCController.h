#pragma once
#ifndef NPCCONTROLLER_H
#define NPCCONTROLLER_H

#include "Agent.h"

class Weapon;

class NPCController : public Agent {
public:
	NPCController();
	~NPCController();
	static unsigned int CreateInstance() { return static_cast<unsigned int>(C_NPCCONTROLLER); }
	virtual void Update(float dt);
	virtual void LateInitialize();
	virtual void Serialize(const json& j);
	virtual void HandleEvent(Event* pEvent);
protected:
	Weapon* m_pWeapon;
};

#endif

