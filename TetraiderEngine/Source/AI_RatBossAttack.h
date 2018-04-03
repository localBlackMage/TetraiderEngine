/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: AI_RatBossAttack.h
Purpose: Attack behavior of agent
Language: C++
Project: GAM541
Author: Hyoyup Chung
Creation date: 4/03/18
- End Header --------------------------------------------------------*/
#pragma once
#ifndef AI_RATBOSS_ATTACK_H
#define AI_RATBOSS_ATTACK_H

class AI_RatBossAttack : public AI_State {
public:
	AI_RatBossAttack();
	~AI_RatBossAttack();
	static AI_State* CreateInstance() { return new AI_RatBossAttack(); }

	virtual void OnEnter();
	virtual void OnUpdate(float);
	virtual void OnExit(); 
	virtual void HandleEvent(Event* pEvent);
	virtual void Serialize(const json& j);

private:
	int m_attackCounter;
	int m_attackMaxLimit;
	int m_attackMinLimit;

	float m_steerTimer;
	float m_steerFor;
};

#endif