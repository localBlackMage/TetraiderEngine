/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: AI_EnemyBatReaction.h
Purpose: Retreat behavior of agent
Language: C++
Project: GAM541
Author: Hyoyup Chung
Creation date: 3/26/18
- End Header --------------------------------------------------------*/

#pragma once
#ifndef AI_ENEMYBAT_REACTION_H
#define AI_ENEMYBAT_REACTION_H

//#include "AI_State.h"

class AI_EnemyBatReaction : public AI_State {
public:
	AI_EnemyBatReaction();
	~AI_EnemyBatReaction();
	static AI_State* CreateInstance() { return new AI_EnemyBatReaction(); }

	virtual void OnEnter();
	virtual void OnUpdate(float);
	virtual void OnExit();
	virtual void HandleEvent(Event* pEvent);

private:
	float idleDuration; // in seconds
	float idledSoFar;
};

#endif