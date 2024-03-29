/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: AI_WalkTowardPlayerIdle.h
Purpose: Idle behavior of agent
Language: C++
Project: GAM541
Author: Hyoyup Chung
Creation date: 4/10/18
- End Header --------------------------------------------------------*/

#pragma once
#ifndef AI_WALKTOWARDPLAYER_IDLE_H
#define AI_WALKTOWARDPLAYER_IDLE_H

//#include "AI_State.h"

class AI_WalkTowardPlayerIdle : public AI_State {
public:
	AI_WalkTowardPlayerIdle();
	~AI_WalkTowardPlayerIdle();
	static AI_State* CreateInstance() { return new AI_WalkTowardPlayerIdle(); }

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