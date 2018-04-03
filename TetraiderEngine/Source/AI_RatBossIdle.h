/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: AI_RatBossIdle.h
Purpose: Idle behavior of agent
Language: C++
Project: GAM541
Author: Hyoyup Chung
Creation date: 2/22/18
- End Header --------------------------------------------------------*/

#pragma once
#ifndef AI_RATBOSS_IDLE_H
#define AI_RATBOSS_IDLE_H

//#include "AI_State.h"

class AI_RatBossIdle : public AI_State {
public:
	AI_RatBossIdle();
	~AI_RatBossIdle();
	static AI_State* CreateInstance() { return new AI_RatBossIdle(); }

	virtual void OnEnter();
	virtual void OnUpdate(float);
	virtual void OnExit();
	virtual void HandleEvent(Event* pEvent);
	virtual void Serialize(const json& j);

private:
	float idleDuration; // in seconds
	float idledSoFar;
};

#endif