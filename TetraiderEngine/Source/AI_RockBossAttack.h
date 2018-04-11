/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: AI_RockBossAttack.h
Purpose: Attack behavior of agent
Language: C++
Project: GAM541
Author: Hyoyup Chung
Creation date: 4/4/18
- End Header --------------------------------------------------------*/
#pragma once
#ifndef AI_ROCKBOSS_ATTACK_H
#define AI_ROCKBOSS_ATTACK_H

//#include "AI_State.h"

class AI_RockBossAttack : public AI_State {
public:
	AI_RockBossAttack();
	~AI_RockBossAttack();
	static AI_State* CreateInstance() { return new AI_RockBossAttack(); }

	virtual void OnEnter();
	virtual void OnUpdate(float);
	virtual void OnExit();
	virtual void HandleEvent(Event* pEvent);
	virtual void Serialize(const json& j);

private:
	float triedSofar;
	float tryTime;
	int m_phase2Attacks;
};

#endif