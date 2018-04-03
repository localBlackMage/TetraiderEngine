/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: AI_RatBossEngage.h
Purpose: Engage behavior of agent
Language: C++
Project: GAM541
Author: Hyoyup Chung
Creation date: 4/03/18
- End Header --------------------------------------------------------*/

#pragma once
#ifndef AI_RATBOSS_ENGAGE_H
#define AI_RATBOSS_ENGAGE_H

class AI_RatBossEngage : public AI_State {
public:
	AI_RatBossEngage();
	~AI_RatBossEngage();
	static AI_State* CreateInstance() { return new AI_RatBossEngage(); }

	virtual void OnEnter();
	virtual void OnUpdate(float);
	virtual void OnExit();
	virtual void HandleEvent(Event* pEvent);
	virtual void Serialize(const json& j);

private:
	float tryTimeDuration; // in seconds
	float engageTimer;
	float minimumEngage;
	float triedMovingSoFar;
	float sinceEngage;
	
};

#endif