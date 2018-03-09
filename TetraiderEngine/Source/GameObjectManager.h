#pragma once
#ifndef GAME_OBJECT_MANAGER_H
#define GAME_OBJECT_MANAGER_H

//#include "GameObjectTags.h"
//#include "ComponentFactory.h"
//#include "Subscriber.h"
//#include "Layers.h"
//#include <string>
//#include <vector>
//#include <iostream>


// Forward declarations
//class GameObject;
//class Event;

class GameObjectLayer {
private:
	std::vector<GameObject*> m_layerObjects;
public:
	GameObjectLayer() {};
	~GameObjectLayer() {};
	GameObjectLayer(const GameObjectLayer & rhs);
	void operator=(const GameObjectLayer & rhs);

	void RenderLayer(GameObject* camera);
	void ReSortLayer();
	void AddToLayer(GameObject* pGO);
	void RemoveFromLayer(GameObject* pGO);
	void ClearLayer();
};


class GameObjectManager: public Subscriber
{
private:
	unsigned int m_currentId;
	std::vector<GameObject*> m_GameObjectsQueue;
	std::vector<GameObject*> m_pCameras;
	GameObject* m_pPrimaryCamera;
	std::vector<GameObject*> mGameObjectsWithHealthComponents;

	GameObjectLayer m_layers[RENDER_LAYER::L_NUM_LAYERS];
	std::vector<GameObject*> mGameObjects;
	
	void SetGameObjectTag(std::string tag, GameObject* pGO);
	void SetGameObjectTag(GameObjectTag tag, GameObject* pGO);
	void SetGameObjectLayer(std::string layer, GameObject* pGO);
	void AddGameObject(GameObject* pGO);
	void AddGameObjectToQueue(GameObject* pGO);
	void AddGameObjectsFromQueueToMainVector();
	void RemoveGameObjectsFromHealthList(GameObject* pGO);
	
	void DestroyGameObjects();
	void HandleEvent(Event *pEvent);

	void _InsertGameObjectIntoList(GameObject* pGO);
	GameObject* m_pPlayerReference;
public:
	GameObjectManager();
	~GameObjectManager();
	GameObjectManager(const GameObjectManager &) = delete;
	void operator=(const GameObjectManager &) = delete;

	void Update(float dt);
	void LateUpdate(float dt);
	void UpdateForLevelEditor(float dt);
	void LateUpdateForLevelEditor(float dt);
	void RenderGameObjects();
	void DestroyAllGameObjects();
	void UpdateStatus();
	const std::vector<GameObject*>& GetObjectsWithHealthComponents() {	return mGameObjectsWithHealthComponents; }
	const GameObject* GetPlayer() { return m_pPlayerReference; }

	// TODO: This method is a temporary hack for getting a camera, a more thorough 
	// architecture should be implemented
	GameObject* GetCamera(int camIndex) const { return m_pCameras[camIndex]; }
	void SetPrimaryCamera(GameObject* pCameraGO) { m_pPrimaryCamera = pCameraGO; }
	GameObject* GetPrimaryCamera() const { return m_pPrimaryCamera; }

	GameObject* CreateGameObject(const std::string& name, bool isCreateAtPos = false, const Vector3D& position = Vector3D());
	GameObjectTag GameObjectManager::FindTagWithString(std::string tag);
	GameObject* FindObjectWithTag(GameObjectTag tag);
	RENDER_LAYER GetLayerFromString(std::string layerName);
};

#endif