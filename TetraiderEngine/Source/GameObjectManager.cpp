#include "GameObjectManager.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Sorting.h"
#include "TetraiderAPI.h"
#include <fstream>
#include <iostream>
#include <unordered_map>

static std::unordered_map<std::string, LAYER> LAYER_STRINGS = {
	{ "N/A", LAYER::NOT_RENDERED },
	{ "BG", LAYER::BACKGROUND },
	{ "1", LAYER::ONE },
	{ "2", LAYER::TWO },
	{ "3", LAYER::TREE },
	{ "UI", LAYER::UI }
};

using namespace JsonReader;
static const std::string COMPONENTS = "COMPONENTS";

static bool SortTransformY(GameObject*left, GameObject*right) 
{
	Transform* tLeft = left->GetComponent<Transform>(ComponentType::C_Transform);
	Transform* tRight = right->GetComponent<Transform>(ComponentType::C_Transform);

	return tLeft->GetPosition().y > tRight->GetPosition().y;
}



void GameObjectLayer::RenderLayer(GameObject* camera)
{
	for (GameObject* pGO : m_layerObjects) {
		if (pGO->m_isActive)
			T_RENDERER.RenderGameObject(*camera, *pGO);
	}
}

void GameObjectLayer::AddToLayer(GameObject * pGO)
{
	m_layerObjects.push_back(pGO);
	Sorting::InsertionSort(m_layerObjects, &SortTransformY);
}

void GameObjectLayer::RemoveFromLayer(GameObject * pGO)
{
	m_layerObjects.erase(
		std::remove(m_layerObjects.begin(), m_layerObjects.end(), pGO),
		m_layerObjects.end()
	);
}

void GameObjectLayer::ClearLayer()
{
	m_layerObjects.clear();
}







GameObjectManager::GameObjectManager() :
	m_currentId(0)
{
	for (int i = 0; i < LAYER::NUM_LAYERS; ++i) {
		m_layers[i] = new GameObjectLayer(); // TODO: Move to memory manager
	}
}

GameObjectManager::~GameObjectManager() {
	DestroyAllGameObjects();
	mGameObjects.clear();

	for (int i = 0; i < LAYER::NUM_LAYERS; ++i) {
		delete m_layers[i]; // TODO: Move to memory manager
	}
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
	//for (GameObject* GO : mGameObjects) {
	//	if (GO->m_isActive)
	//		T_RENDERER.RenderGameObject(*m_pCamera, *GO);
	//}

	for (int layer = 0; layer < NUM_LAYERS; ++layer) {
		m_layers[layer]->RenderLayer(m_pCamera);
	}
}

void GameObjectManager::AddGameObject(GameObject* pGO) {
	_InsertGameObjectIntoList(pGO);

	// TO DO
	/*if (pGO->GetComponent(CT_UI_ELEMENT))
		mainManager.pUI_Manager->AddGameObject(pGO);*/
	if (pGO->HasComponent(ComponentType::C_Body)) {
		T_PHYSICS.AddGameObject(pGO);
	}
}

void GameObjectManager::DestroyGameObjects() {
	for (std::vector<GameObject*>::iterator it = mGameObjects.begin(); it != mGameObjects.end();) {
		if ((*it)->m_isDestroy) {
			m_layers[(*it)->GetLayer()]->RemoveFromLayer(*it);
			T_PHYSICS.RemoveGameObject(*it);
			delete (*it); // TODO: Move to memory manager
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

	GameObject *pGameObject = new GameObject(++m_currentId); // TODO: Move to memory manager
	SetGameObjectTag(ParseString(j, "Tag"), pGameObject);

	// TODO: Find a cleaner way to do this?
	if (pGameObject->m_tag == GameObjectTag::T_Camera)	m_pCamera = pGameObject;

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

void GameObjectManager::SetGameObjectTag(GameObjectTag tag, GameObject * pGO)
{
	pGO->m_tag = tag;
}

GameObjectTag GameObjectManager::FindTagWithString(std::string tag) {
	// TODO: Convert Tags to something better, try the trick mentioned by Prof. Rabin
	if (tag == "Player") return GameObjectTag::T_Player;
	else if (tag == "Camera") return GameObjectTag::T_Camera;
	else if (tag == "Enemy") return GameObjectTag::T_Enemy;
	else return GameObjectTag::NONE;
}

void GameObjectManager::HandleEvent(Event *pEvent) {
	if (pEvent->Type() == EVENT_OnLevelInitialized) {
		UpdateStatus();
	}
}



void GameObjectManager::_InsertGameObjectIntoList(GameObject * pGO)
{
	mGameObjects.push_back(pGO);

	pGO->SetLayer(LAYER::BACKGROUND);
	if (pGO->GetLayer() != LAYER::NOT_RENDERED)
		m_layers[pGO->GetLayer()]->AddToLayer(pGO);
}

LAYER GameObjectManager::_GetLayerFromString(std::string layerName)
{
	return LAYER_STRINGS[layerName];
}
