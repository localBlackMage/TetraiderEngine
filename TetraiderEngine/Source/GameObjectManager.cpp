#include "GameObjectManager.h"
#include "PhysicsManager.h"
#include "GameObject.h"
#include "Component.h"
#include "GameConfig.h"
#include <fstream>
#include <iostream>

using namespace JsonReader;
static const std::string COMPONENTS = "COMPONENTS";

GameObjectManager::GameObjectManager() :
	m_currentId(0),
	renderMngr(RenderManager::GetInstance())
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
			renderMngr.RenderGameObject(*m_pCamera, *GO);
	}
}

void GameObjectManager::AddGameObject(GameObject* pGO) {
	mGameObjects.push_back(pGO);

	// TO DO
	/*if (pGO->GetComponent(CT_UI_ELEMENT))
		mainManager.pUI_Manager->AddGameObject(pGO);*/
	if (pGO->GetComponent(ComponentType::Body)) {
		PhysicsManager& physicsMngr = PhysicsManager::GetInstance();
		physicsMngr.AddGameObject(pGO);
	}
}

void GameObjectManager::DestroyGameObjects() {
	for (std::vector<GameObject*>::iterator it = mGameObjects.begin(); it != mGameObjects.end();) {
		if ((*it)->m_isDestroy) {
			// TODO
			// Unsubscribe from events
			// UI manager list
			PhysicsManager& physicsMngr = PhysicsManager::GetInstance();
			physicsMngr.RemoveGameObject(*it);
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
	GameConfig& gameConfig = GameConfig::GetInstance();
	std::string s = gameConfig.m_prefabsDir + name + ".json";
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
	// TODO: Convert Tags to something better, try the trick mentioned by Prof. Rabin
	if (tag == "Player")
		pGO->m_tag = GameObjectTag::Player;
	else if (tag == "Camera")
		pGO->m_tag = GameObjectTag::Camera;
	else
		pGO->m_tag = GameObjectTag::NONE;
}
