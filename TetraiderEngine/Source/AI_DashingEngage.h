/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: AI_DashingEngage.h
Purpose: DashingEngage behavior of agent
Language: C++
Project: GAM541
Author: Hyoyup Chung
Creation date: 2/22/18
- End Header --------------------------------------------------------*/

#pragma once
#ifndef AI_DASHINGENGAGE_H
#define AI_DASHINGENGAGE_H

//#include "AI_State.h"

class AI_DashingEngage : public AI_State {
public:
	AI_DashingEngage();
	~AI_DashingEngage();
	static AI_State* CreateInstance() { return new AI_DashingEngage(); }

	virtual void OnEnter();
	virtual void OnUpdate(float);
	virtual void OnExit();
	virtual void HandleEvent(Event* pEvent);

private:
	float engageTimer;
	float sinceEngage;
	
};

#endif