/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: AI_HitNRunRetreat.h
Purpose: Retreat behavior of agent
Language: C++
Project: GAM541
Author: Hyoyup Chung
Creation date: 2/22/18
- End Header --------------------------------------------------------*/

#pragma once
#ifndef AI_HITNRUN_RETREAT_H
#define AI_HITNRUN_RETREAT_H

//#include "AI_State.h"

class AI_HitNRunRetreat : public AI_State {
public:
	AI_HitNRunRetreat();
	~AI_HitNRunRetreat();
	static AI_State* CreateInstance() { return new AI_HitNRunRetreat(); }

	virtual void OnEnter();
	virtual void OnUpdate(float);
	virtual void OnExit();
	virtual void HandleEvent(Event* pEvent);

private:
	float minDistanceToPlayer;
	float minimumRetreat;
	float tryTimeDuration;
	float sinceRetreat;
	float triedMovingSoFar;
};

#endif