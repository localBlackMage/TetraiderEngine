/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: AI_Attack.h
Purpose: Attack behavior of agent
Language: C++
Project: GAM541
Author: Hyoyup Chung
Creation date: 2/22/18
- End Header --------------------------------------------------------*/
#pragma once
#ifndef AI_RANGEATTACK_H
#define AI_RANGEATTACK_H

//#include "AI_State.h"

class AI_RangeAttack : public AI_State {
public:
	AI_RangeAttack();
	~AI_RangeAttack();
	static AI_State* CreateInstance() { return new AI_RangeAttack(); }

	virtual void OnEnter();
	virtual void OnUpdate(float);
	virtual void OnExit(); 
	virtual void HandleEvent(Event* pEvent);
	virtual void Serialize(const json& j);

private:
	int m_attackCounter;
	int m_attackMaxLimit;
	int m_attackMinLimit;
};

#endif