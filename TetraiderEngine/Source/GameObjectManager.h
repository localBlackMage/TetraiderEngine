#pragma once
#ifndef GAME_OBJECT_MANAGER_H
#define GAME_OBJECT_MANAGER_H

#include "GameObjectTags.h"
#include "ComponentFactory.h"
#include "Subscriber.h"
#include <string>
#include <vector>

enum LAYER {
	BACKGROUND = 0,
	ONE,
	TWO,
	TREE,
	UI,
	NUM_LAYERS,
	NOT_RENDERED
};

// Forward declarations
class GameObject;
class Event;

class GameObjectLayer {
private:
	std::vector<GameObject*> m_layerObjects;
public:
	GameObjectLayer() {};
	~GameObjectLayer() {};
	GameObjectLayer(const GameObjectLayer &) = delete;
	void operator=(const GameObjectLayer &) = delete;

	void RenderLayer(GameObject* camera);
	void AddToLayer(GameObject* pGO);
	void RemoveFromLayer(GameObject* pGO);
	void ClearLayer();
};

class GameObjectManager: public Subscriber
{
private:
	unsigned int m_currentId;
	ComponentFactory componentFactory;
	std::vector<GameObject*> m_GameObjectsQueue;
	GameObject* m_pCamera;

	GameObjectLayer* m_layers[LAYER::NUM_LAYERS];
	std::vector<GameObject*> mGameObjects;
	
	void SetGameObjectTag(std::string tag, GameObject* pGO);
	void SetGameObjectTag(GameObjectTag tag, GameObject* pGO);
	void AddGameObject(GameObject* pGO);
	void AddGameObjectToQueue(GameObject* pGO);
	void AddGameObjectsFromQueueToMainVector();
	
	void DestroyGameObjects();
	void HandleEvent(Event *pEvent);

	void _InsertGameObjectIntoList(GameObject* pGO);
	LAYER _GetLayerFromString(std::string layerName);
public:
	void DestroyAllGameObjects();
	GameObjectManager();
	~GameObjectManager();
	GameObjectManager(const GameObjectManager &) = delete;
	void operator=(const GameObjectManager &) = delete;

	void Update(float dt);
	void LateUpdate(float dt);
	void RenderGameObjects();

	GameObject* CreateGameObject(std::string name);
	GameObjectTag GameObjectManager::FindTagWithString(std::string tag);
	void UpdateStatus();
	// Component* AddComponentToGameObject(GameObject* pGO, json j);
	GameObject* FindObjectWithTag(GameObjectTag tag);
	
	GameObject* GetActiveCamera();
};

#endif