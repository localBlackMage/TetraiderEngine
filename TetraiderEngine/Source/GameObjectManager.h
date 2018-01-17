/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObjectManager.h
Purpose: Object pool manager - stores a pool of GameObject instances, handles creating instances and destroying them.
Language: C++
Project: GAM541
Author: Holden Profit
Creation date: 1/17/18
- End Header --------------------------------------------------------*/

#pragma once

#ifndef GAME_OBJECT_MANAGER_H
#define GAME_OBJECT_MANAGER_H

class GameObjectManager
{
private:
	GameObjectManager();
	~GameObjectManager();

public:
	GameObjectManager(const GameObjectManager &) = delete;
	void operator=(const GameObjectManager &) = delete;

	static GameObjectManager& GetInstance()
	{
		static GameObjectManager instance;
		return instance;
	}
};

#endif