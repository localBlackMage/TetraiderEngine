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
#ifndef AI_STATE_FACTORY_H
#define AI_STATE_FACTORY_H

// forward declaration
class AI_State;

class AIStateFactory{
public:
	AIStateFactory();
	~AIStateFactory(){};

	AI_State* CreateState(std::string state);
private:
	typedef AI_State* CreationFunction();
	std::unordered_map <std::string, CreationFunction*> m_CreationFunctions;
};

#endif