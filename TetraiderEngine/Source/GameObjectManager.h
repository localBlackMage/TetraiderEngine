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

#include <string>
#include <vector>
#include "ComponentFactory.h"
#include "JsonReader.h"

// Forward declarations
class GameObject;
class Component;
enum class GameObjectTag;

using json = nlohmann::json;


class GameObjectManager
{
private:
	GameObjectManager();
	~GameObjectManager();

	ComponentFactory componentFactory;
	std::vector<GameObject*> m_GameObjectsQueue;
	void SetGameObjectTag(std::string tag, GameObject* pGO);
	void AddGameObject(GameObject* pGO);
	void AddGameObjectToQueue(GameObject* pGO);
	void AddGameObjectsFromQueueToMainVector();
	void DestroyAllGameObjects();
	void DestroyGameObjects();
public:
	GameObjectManager(const GameObjectManager &) = delete;
	void operator=(const GameObjectManager &) = delete;

	static GameObjectManager& GetInstance()
	{
		static GameObjectManager instance;
		return instance;
	}

	void Update(float dt);
	void LateUpdate(float dt);
	GameObject* CreateGameObject(std::string name);
	void UpdateStatus();
	Component* AddComponentToGameObject(GameObject* pGO, json j);
	GameObject* FindObjectWithTag(GameObjectTag tag);
	std::vector<GameObject*> mGameObjects;
};

#endif