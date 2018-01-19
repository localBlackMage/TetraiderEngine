#include "GameObjectManager.h"
#include "GameObject.h"
#include "Component.h"
#include <fstream>
#include <iostream>

using namespace JsonReader;

GameObjectManager::GameObjectManager() {}

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
		// if (gameObject->m_isActive)
			// gameObject->LateUpdate(dt);
	}
}

void GameObjectManager::AddGameObject(GameObject* pGO) {
	mGameObjects.push_back(pGO);

	// TO DO
	/*if (pGO->GetComponent(CT_UI_ELEMENT))
		mainManager.pUI_Manager->AddGameObject(pGO);
	if (pGO->GetComponent(CT_BODY))
		mainManager.pPhysicsManager->AddGameObject(pGO);*/
}

void GameObjectManager::DestroyGameObjects() {
	for (std::vector<GameObject*>::iterator it = mGameObjects.begin(); it != mGameObjects.end();) {
		if ((*it)->m_isDestroy) {
			// TODO
			// Unsubscribe from events
			// Remove from physics and UI manager list
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

	json j = OpenJsonFile(name);

	GameObject *pGameObject = new GameObject;
	SetGameObjectTag(ParseString(j, "Tag"), pGameObject);

	int size = j["COMPONENTS"].size();
	for (int i = 0; i < size; ++i) {
		Component* pComponent = componentFactory.CreateComponent(ParseString(j["COMPONENTS"][i], "Component"));
		pGameObject->AddComponent(pComponent);
		pComponent->Serialize(j["COMPONENTS"][i]);
	}

	pGameObject->LateInitialize();

	AddGameObjectToQueue(pGameObject);

	return pGameObject;
}

Component* GameObjectManager::AddComponentToGameObject(GameObject* pGO, json j) {
	Component* pComponent = componentFactory.CreateComponent(ParseString(j, "Component"));
	pGO->AddComponent(pComponent);
	pComponent->Serialize(j);
	pComponent->LateInitialize();
	return pComponent;
}

void GameObjectManager::SetGameObjectTag(std::string tag, GameObject* pGO) {
	if (tag == "Tag_Player")
		pGO->m_tag = GameObjectTag::Player;
	else
		pGO->m_tag = GameObjectTag::NONE;
}
