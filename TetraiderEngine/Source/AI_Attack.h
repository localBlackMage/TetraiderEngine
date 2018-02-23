/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: AI_Attack.h
Purpose: Attack behavior of agent
Language: C++
Project: GAM541
Author: Hyoyup Chung
Creation date: 2/22/18
- End Header --------------------------------------------------------*/
#pragma once
#ifndef AI_ATTACK_H
#define AI_ATTACK_H

#include "AI_State.h"

class AI_Attack : public AI_State {
public:
	AI_Attack();
	~AI_Attack();
	static AI_State* CreateInstance() { return new AI_Attack(); }

	virtual void OnEnter();
	virtual void OnUpdate();
	virtual void OnExit();

private:
};

#endif