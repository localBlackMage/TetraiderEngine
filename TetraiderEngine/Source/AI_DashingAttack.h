/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: AI_DashingAttack.h
Purpose: Attack behavior of agent
Language: C++
Project: GAM541
Author: Hyoyup Chung
Creation date: 2/22/18
- End Header --------------------------------------------------------*/
#pragma once
#ifndef AI_DASHINGATTACK_H
#define AI_DASHINGATTACK_H

//#include "AI_State.h"

class AI_DashingAttack : public AI_State {
public:
	AI_DashingAttack();
	~AI_DashingAttack();
	static AI_State* CreateInstance() { return new AI_DashingAttack(); }

	virtual void OnEnter();
	virtual void OnUpdate(float);
	virtual void OnExit();
	virtual void HandleEvent(Event* pEvent);

private:
	float triedSofar;
	float tryTime;
};

#endif