/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: AI_MoveToRandomPointInZone.h
Purpose: Engage behavior of agent
Language: C++
Project: GAM541
Author: Hyoyup Chung
Creation date: 2/22/18
- End Header --------------------------------------------------------*/

#pragma once
#ifndef AI_MOVETORANDOMPOINTINZONE_H
#define AI_MOVETORANDOMPOINTINZONE_H

//#include "AI_State.h"

class AI_MoveToRandomPointInZone : public AI_State {
public:
	AI_MoveToRandomPointInZone();
	~AI_MoveToRandomPointInZone();
	static AI_State* CreateInstance() { return new AI_MoveToRandomPointInZone(); }

	virtual void OnEnter();
	virtual void OnUpdate(float);
	virtual void OnExit();
	virtual void HandleEvent(Event* pEvent);

private:
	float triedSofar;
	float tryTime;
};

#endif