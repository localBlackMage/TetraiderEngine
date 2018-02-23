/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: AI_Idle.h
Purpose: Idle behavior of agent
Language: C++
Project: GAM541
Author: Hyoyup Chung
Creation date: 2/22/18
- End Header --------------------------------------------------------*/

#pragma once
#ifndef AI_IDLE_H
#define AI_IDLE_H

#include "AI_State.h"

class AI_Idle : public AI_State {
public:
	AI_Idle();
	~AI_Idle();
	static AI_State* CreateInstance() { return new AI_Idle(); }

	virtual void OnEnter();
	virtual void OnUpdate();
	virtual void OnExit();

private:
};

#endif