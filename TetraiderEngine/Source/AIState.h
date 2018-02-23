/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: AIComponent.h
Purpose: AI component class for NPC Controller component inside the gameobject
Language: C++
Project: GAM541
Author: Hyoyup Chung
Creation date: 2/22/18
- End Header --------------------------------------------------------*/
#pragma once
#ifndef AI_STATE_H
#define AI_STATE_H

#include "AIStateTypes.h"
#include "Subscriber.h"
#include "JsonReader.h"
#include "Agent.h"
#include <assert.h>

using json = nlohmann::json;
using namespace JsonReader;

// forward declaration
class Agent;
class Event;

class AIState: public Subscriber{
protected:
	AIStateType m_StateType;
public:
	AIState(AIStateType _type): m_StateType(_type){};
	virtual ~AIState(){};
	AIStateType StateType() const {return m_StateType;}

	virtual void OnEnter() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnExit() = 0;

	Agent *pNPCComp;
};

#endif