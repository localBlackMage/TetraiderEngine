/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: AI_Engage.h
Purpose: Engage behavior of agent
Language: C++
Project: GAM541
Author: Hyoyup Chung
Creation date: 2/22/18
- End Header --------------------------------------------------------*/

#pragma once
#ifndef AI_ENGAGE_H
#define AI_ENGAGE_H

#include "AI_State.h"

class AI_Engage : public AI_State {
public:
	AI_Engage();
	~AI_Engage();
	static AI_State* CreateInstance() { return new AI_Engage(); }

	virtual void OnEnter();
	virtual void OnUpdate(float);
	virtual void OnExit();

private:
	int tryTimeDuration; // in seconds
	int engageTimer;
	int minimumEngage;
	float triedMovingSoFar;
	float sinceEngage;
	
};

#endif