/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: AI_RockBossEngage.h
Purpose: DashingEngage behavior of agent
Language: C++
Project: GAM541
Author: Hyoyup Chung
Creation date: 4/4/18
- End Header --------------------------------------------------------*/

#pragma once
#ifndef AI_ROCKBOSS_ENGAGE_H
#define AI_ROCKBOSS_ENGAGE_H

class AI_RockBossEngage : public AI_State {
public:
	AI_RockBossEngage();
	~AI_RockBossEngage();
	static AI_State* CreateInstance() { return new AI_RockBossEngage(); }

	virtual void OnEnter();
	virtual void OnUpdate(float);
	virtual void OnExit();
	virtual void HandleEvent(Event* pEvent);
	virtual void Serialize(const json& j);

private:
	float engageTimer;
	float sinceEngage;
	float bombAngleOffset;
	
};

#endif