/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: AI_HitNRunAttack.h
Purpose: Hit and Run Attack behavior of agent
Language: C++
Project: GAM541
Author: Hyoyup Chung
Creation date: 3/13/18
- End Header --------------------------------------------------------*/
#pragma once
#ifndef AI_HITNRUN_ATTACK_H
#define AI_HITNRUN_ATTACK_H

//#include "AI_State.h"

class AI_HitNRunAttack : public AI_State {
public:
	AI_HitNRunAttack();
	~AI_HitNRunAttack();
	static AI_State* CreateInstance() { return new AI_HitNRunAttack(); }

	virtual void OnEnter();
	virtual void OnUpdate(float);
	virtual void OnExit(); 
	virtual void HandleEvent(Event* pEvent);

private:
	int attackCounter;
	int attackLimit;
	float minDistanceToPlayer;
};

#endif