#include "GameObjectManager.h"
#include "GameObject.h"
#include "Component.h"
#include <fstream>
#include <iostream>

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
	std::string s = "Metadata\\Archetypes\\" + name + ".json";
	std::ifstream i(s);

	try {
		json j;
		i >> j;
		i.close();

		GameObject *pGameObject = new GameObject;
		SetGameObjectTag(j.at("Tag").get<std::string>(), pGameObject);

		int size = j["COMPONENTS"].size();
		for (int i = 0; i < size; ++i) {
			Component* pComponent = componentFactory.CreateComponent(j["COMPONENTS"][i].at("Component").get<std::string>());
			pGameObject->AddComponent(pComponent);
			pComponent->Serialize(j["COMPONENTS"][i]);
		}

		pGameObject->LateInitialize();

		AddGameObjectToQueue(pGameObject);

		return pGameObject;
	}
	catch (json::parse_error &e) {
		std::cerr << e.what() << " " << s << std::endl;
		return 0;
	}
}

Component* GameObjectManager::AddComponentToGameObject(GameObject* pGO, json j) {
	Component* pComponent = componentFactory.CreateComponent(j.at("Component").get<std::string>());
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
