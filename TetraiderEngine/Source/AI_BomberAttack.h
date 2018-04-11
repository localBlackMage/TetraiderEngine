/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: AI_BomberAttack.h
Purpose: Attack behavior of agent
Language: C++
Project: GAM541
Author: Hyoyup Chung
Creation date: 3/17/18
- End Header --------------------------------------------------------*/
#pragma once
#ifndef AI_BOMBER_ATTACK_H
#define AI_BOMBER_ATTACK_H

//#include "AI_State.h"

class AI_BomberAttack : public AI_State {
public:
	AI_BomberAttack();
	~AI_BomberAttack();
	static AI_State* CreateInstance() { return new AI_BomberAttack(); }

	virtual void OnEnter();
	virtual void OnUpdate(float);
	virtual void OnExit(); 
	virtual void HandleEvent(Event* pEvent);
	virtual void Serialize(const json& j);

private:
	int m_attackCounter;
	int m_attackMaxLimit;
	int m_attackMinLimit;
	float idleTime;
	float idleDuration;
};

#endif