/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: AI_HitNRunEngage.h
Purpose: Engage behavior of agent
Language: C++
Project: GAM541
Author: Hyoyup Chung
Creation date: 3/13/18
- End Header --------------------------------------------------------*/

#pragma once
#ifndef AI_HITNRUN_ENGAGE_H
#define AI_HITNRUN_ENGAGE_H

class AI_HitNRunEngage : public AI_State {
public:
	AI_HitNRunEngage();
	~AI_HitNRunEngage();
	static AI_State* CreateInstance() { return new AI_HitNRunEngage(); }

	virtual void OnEnter();
	virtual void OnUpdate(float);
	virtual void OnExit();
	virtual void HandleEvent(Event* pEvent);

private:
	float tryTimeDuration; // in seconds
	float engageTimer;
	float minimumEngage;
	float triedMovingSoFar;
	float sinceEngage;
	float minDistanceToPlayer;
};

#endif