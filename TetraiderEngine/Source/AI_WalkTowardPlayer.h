/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: AI_WalkTowardPlayer.h
Purpose: Attack behavior of agent
Language: C++
Project: GAM541
Author: Hyoyup Chung
Creation date: 2/25/18
- End Header --------------------------------------------------------*/
#pragma once
#ifndef AI_WALKTOWARDPLAYER_H
#define AI_WALKTOWARDPLAYER_H

#include "AI_State.h"

class AI_WalkTowardPlayer : public AI_State {
public:
	AI_WalkTowardPlayer();
	~AI_WalkTowardPlayer();
	static AI_State* CreateInstance() { return new AI_WalkTowardPlayer(); }

	virtual void OnEnter();
	virtual void OnUpdate(float);
	virtual void OnExit();
	virtual void HandleEvent(Event* pEvent);

private:
	int idleDuration; // in seconds
	float idledSoFar;
};

#endif