/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef AI_STATE_H
#define AI_STATE_H

class NPCController;

#define OUT_OF_SIGHTTIME 1.0f

class AI_State: public Subscriber{
protected:
	AIStateType m_StateType;
	float outOfSightTime;
public:
	AI_State(AIStateType _type): m_StateType(_type){}
	virtual ~AI_State(){}

	virtual void OnEnter() = 0;
	virtual void OnUpdate(float dt) = 0;
	virtual void OnExit() = 0;
	virtual void HandleEvent(Event* pEvent) = 0;
	virtual void Serialize(const json& j) = 0;

	AIStateType StateType() const { return m_StateType; }

	NPCController *pAgent;
};

#endif