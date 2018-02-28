/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: AI_DashingStunned.h
Purpose: DashingEngage behavior of agent
Language: C++
Project: GAM541
Author: Hyoyup Chung
Creation date: 2/27/18
- End Header --------------------------------------------------------*/

#pragma once
#ifndef AI_DASHINGSTUNNED_H
#define AI_DASHINGSTUNNED_H

#include "AI_State.h"

class AI_DashingStunned : public AI_State {
public:
	AI_DashingStunned();
	~AI_DashingStunned();
	static AI_State* CreateInstance() { return new AI_DashingStunned(); }

	virtual void OnEnter();
	virtual void OnUpdate(float);
	virtual void OnExit();
	virtual void HandleEvent(Event* pEvent);

private:
	float triedSofar;
	float tryTime;
	
};

#endif