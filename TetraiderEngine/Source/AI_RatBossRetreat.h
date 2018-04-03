/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: AI_RatBossRetreat.h
Purpose: Retreat behavior of agent
Language: C++
Project: GAM541
Author: Hyoyup Chung
Creation date: 4/03/18
- End Header --------------------------------------------------------*/

#pragma once
#ifndef AI_RATBOSS_RETREAT_H
#define AI_RATBOSS_RETREAT_H

//#include "AI_State.h"

class AI_RatBossRetreat : public AI_State {
public:
	AI_RatBossRetreat();
	~AI_RatBossRetreat();
	static AI_State* CreateInstance() { return new AI_RatBossRetreat(); }

	virtual void OnEnter();
	virtual void OnUpdate(float);
	virtual void OnExit();
	virtual void HandleEvent(Event* pEvent);
	virtual void Serialize(const json& j);

private:
	float m_retreatFor;
	float m_retreatTimer;
};

#endif