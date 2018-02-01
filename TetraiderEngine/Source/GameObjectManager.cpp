#include "GameObjectManager.h"
#include "GameObject.h"
#include "Component.h"
#include <fstream>
#include <iostream>
#include "TetraiderAPI.h"

using namespace JsonReader;
static const std::string COMPONENTS = "COMPONENTS";

GameObjectManager::GameObjectManager() :
	m_currentId(0)
{}

GameObjectManager::~GameObjectManager() {
	DestroyAllGameObjects();
	mGameObjects.clear();
}

void GameObjectManager::Update(float dt) {
	for (auto gameObject : mGameObjects) {
		if (gameObject->m_isActive)
			gameObject->Update(dt);
	}
}

// Adds newly created objects to the main vector and destroys all gameobject with destroy flag = true
void GameObjectManager::UpdateStatus() {
	AddGameObjectsFromQueueToMainVector();
	DestroyGameObjects();
}

void GameObjectManager::LateUpdate(float dt) {
	for (auto gameObject : mGameObjects) {
		 if (gameObject->m_isActive)
			 gameObject->LateUpdate(dt);
	}
}

void GameObjectManager::RenderGameObjects()
{
	for (GameObject* GO : mGameObjects) {
		if (GO->m_isActive)
			T_RENDERER.RenderGameObject(*m_pCamera, *GO);
	}
}

void GameObjectManager::AddGameObject(GameObject* pGO) {
	mGameObjects.push_back(pGO);

	// TO DO
	/*if (pGO->GetComponent(CT_UI_ELEMENT))
		mainManager.pUI_Manager->AddGameObject(pGO);*/
	if (pGO->GetComponent(ComponentType::Body)) {
		T_PHYSICS.AddGameObject(pGO);
	}
}

void GameObjectManager::DestroyGameObjects() {
	for (std::vector<GameObject*>::iterator it = mGameObjects.begin(); it != mGameObjects.end();) {
		if ((*it)->m_isDestroy) {
			// TODO
			// Unsubscribe from events
			// UI manager list
			T_PHYSICS.RemoveGameObject(*it);
			delete (*it);
			it = mGameObjects.erase(it);
		}
		else {
			++it;
		}
	}
}

void GameObjectManager::DestroyAllGameObjects() {
	for (auto gameObject : mGameObjects) {
		gameObject->Destroy();
	}

	DestroyGameObjects();
}

GameObject* GameObjectManager::FindObjectWithTag(GameObjectTag tag) {
	for (auto gameObject : mGameObjects) {
		if (gameObject->m_tag == tag)
			return gameObject;
	}

	return 0;
}

GameObject * GameObjectManager::GetActiveCamera()
{
	return m_pCamera;
}

void GameObjectManager::AddGameObjectToQueue(GameObject* pGO) {
	m_GameObjectsQueue.push_back(pGO);
}

void GameObjectManager::AddGameObjectsFromQueueToMainVector() {
	for (auto gameObject : m_GameObjectsQueue) {
		AddGameObject(gameObject);
	}

	m_GameObjectsQueue.clear();
}

GameObject* GameObjectManager::CreateGameObject(std::string name) {
	std::string s = T_GAME_CONFIG.PrefabsDir() + name + ".json";
	json j = OpenJsonFile(s);

	GameObject *pGameObject = new GameObject(++m_currentId);
	SetGameObjectTag(ParseString(j, "Tag"), pGameObject);

	// TODO: Find a cleaner way to do this?
	if (pGameObject->m_tag == GameObjectTag::Camera)	m_pCamera = pGameObject;

	int size = j[COMPONENTS].size();
	for (int i = 0; i < size; ++i) {
		Component* pComponent = componentFactory.CreateComponent(ParseString(j[COMPONENTS][i], "Component"));
		pGameObject->AddComponent(pComponent);
		pComponent->Serialize(j[COMPONENTS][i]);
	}

	pGameObject->LateInitialize();

	AddGameObjectToQueue(pGameObject);

	return pGameObject;
}

/*Component* GameObjectManager::AddComponentToGameObject(GameObject* pGO, json j) {
	Component* pComponent = componentFactory.CreateComponent(ParseString(j, "Component"));
	pGO->AddComponent(pComponent);
	pComponent->Serialize(j);
	pComponent->LateInitialize();
	return pComponent;
}*/

void GameObjectManager::SetGameObjectTag(std::string tag, GameObject* pGO) {
	pGO->m_tag = FindTagWithString(tag);
}

GameObjectTag GameObjectManager::FindTagWithString(std::string tag) {
	// TODO: Convert Tags to something better, try the trick mentioned by Prof. Rabin
	if (tag == "Player") return GameObjectTag::Player;
	else if (tag == "Camera") return GameObjectTag::Camera;
	else if (tag == "Enemy") return GameObjectTag::Enemy;
	else return GameObjectTag::NONE;
}

void GameObjectManager::HandleEvent(Event *pEvent) {
	if (pEvent->Type() == EVENT_OnLevelInitialized) {
		UpdateStatus();
	}
}
