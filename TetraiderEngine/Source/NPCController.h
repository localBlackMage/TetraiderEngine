#pragma once
#ifndef NPCCONTROLLER_H
#define NPCCONTROLLER_H

#include "Agent.h"
#include <unordered_map>
#include "NPCState.h"

class Weapon;

class NPCController : public Agent {
public:
	NPCController();
	~NPCController();
	static Component* CreateInstance() { return new NPCController(); }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void LateInitialize();
	virtual void Serialize(const json& j);
	virtual void HandleEvent(Event* pEvent);
protected:
	Weapon* m_pWeapon;
private:
	std::unordered_map<NPCState, std::string> m_luaScripts;
	NPCState m_currentState;
	NPCState m_previousState;
};

#endif

