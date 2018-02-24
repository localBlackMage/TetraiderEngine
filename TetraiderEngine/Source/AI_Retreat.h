/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: AI_Retreat.h
Purpose: Retreat behavior of agent
Language: C++
Project: GAM541
Author: Hyoyup Chung
Creation date: 2/22/18
- End Header --------------------------------------------------------*/

#pragma once
#ifndef AI_RETREAT_H
#define AI_RETREAT_H

#include "AI_State.h"

class AI_Retreat : public AI_State {
public:
	AI_Retreat();
	~AI_Retreat();
	static AI_State* CreateInstance() { return new AI_Retreat(); }

	virtual void OnEnter();
	virtual void OnUpdate(float);
	virtual void OnExit();

private:
};

#endif