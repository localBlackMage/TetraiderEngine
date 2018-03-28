#pragma once
#ifndef GAME_OBJECT_MANAGER_H
#define GAME_OBJECT_MANAGER_H

#define MAX_LIGHTS 16

class GameObjectLayer {
private:
	std::vector<GameObject*> m_layerObjects;
	std::vector<GameObject*> m_layerLights;

	GLuint m_lightColorsBuffer;
	GLuint m_lightPositionsAndDistancesBuffer;

	static const unsigned short m_size = MAX_LIGHTS * 4;
	float m_lightColors[m_size];				// r, g, b, a
	float m_lightPositionsAndDistances[m_size];	// x, y, z, distance
	float m_lightABs[MAX_LIGHTS * 2];			// a, b

	void _SetLightDataArrays();
public:
	GameObjectLayer();
	~GameObjectLayer() {};
	GameObjectLayer(const GameObjectLayer & rhs);
	void operator=(const GameObjectLayer & rhs);

	void RenderLayer(GameObject* camera);
	void ReSortLayer();
	void AddToLayer(GameObject* pGO);
	void RemoveFromLayer(GameObject* pGO);
	void AddLightToLayer(GameObject* pGO);
	void RemoveLightFromLayer(GameObject* pGO);
	void Update();
	void ClearLayer();

	GLuint GetLightColors()	const { return m_lightColorsBuffer; }
	GLuint GetLightPosAndDist()	const { return m_lightPositionsAndDistancesBuffer; }
	void BindBufferDatas(const Vector3D& pos);
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
	GameObject* m_pPlayerReference;

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
	void _InsertLightIntoLayers(GameObject* pGO);
	void _RenderGameObjectLayers(unsigned int startLayer, unsigned int endLayer);
	void _RenderWithPostProcessing();
	void _RenderWithoutPostProcessing();
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
	void SwitchGameObjectLayer(GameObject* pGO, RENDER_LAYER layer);
	bool IsPlayerAlive();
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