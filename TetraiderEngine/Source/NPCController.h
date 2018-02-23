#pragma once
#ifndef NPCCONTROLLER_H
#define NPCCONTROLLER_H

#include "Agent.h"
#include <unordered_map>
#include "AI_State.h"
#include "AI_StateTypes.h"
#include "AI_StateFactory.h"

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
	float GetSquareDistanceToPlayer();
private:
	AIStateFactory AIStateFactory;
	std::unordered_map<AIStateType, AI_State*> m_AIStates;
	AIStateType m_currentState;
	AIStateType m_previousState;
};

#endif

