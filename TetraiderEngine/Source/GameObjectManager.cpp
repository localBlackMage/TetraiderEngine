#include <Stdafx.h>

static bool LeftYGreaterThanRightY(GameObject*left, GameObject*right)
{
	Transform* tLeft = left->GetComponent<Transform>(ComponentType::C_Transform);
	Transform* tRight = right->GetComponent<Transform>(ComponentType::C_Transform);

	return tLeft->GetPosition().y > tRight->GetPosition().y;
}

static bool LeftYGreaterThanOrEqualToRightY(GameObject*left, GameObject*right)
{
	Transform* tLeft = left->GetComponent<Transform>(ComponentType::C_Transform);
	Transform* tRight = right->GetComponent<Transform>(ComponentType::C_Transform);

	return tLeft->GetPosition().y >= tRight->GetPosition().y;
}

#pragma region GameObjectLayer

GameObjectLayer::GameObjectLayer()
{
	//std::fill(m_lightPositionsAndDistances, m_lightPositionsAndDistances + (MAX_LIGHTS * 4), 0.f);
	std::fill(m_lightColors, m_lightColors + (MAX_LIGHTS * 4), 255);
	m_lightColorsBuffer = TETRA_RENDERER.GenerateStreamingVBO(MAX_LIGHTS * 4 * sizeof(GLubyte));
	m_m_lightPositionsAndDistancesBuffer = TETRA_RENDERER.GenerateStreamingVBO(MAX_LIGHTS * 4 * sizeof(GLfloat));
}

GameObjectLayer::GameObjectLayer(const GameObjectLayer & rhs) : m_layerObjects(rhs.m_layerObjects) {}

void GameObjectLayer::operator=(const GameObjectLayer & rhs)
{
	m_layerObjects = rhs.m_layerObjects;
}

void GameObjectLayer::RenderLayer(GameObject* camera)
{
	for (GameObject* pGO : m_layerObjects) {
		if (pGO->m_isActive && pGO->m_isRender)
			TETRA_RENDERER.RenderGameObject(*camera, *pGO, *this);
	}
}

void GameObjectLayer::ReSortLayer()
{
	Sorting::TopDownMergeSort(m_layerObjects, &LeftYGreaterThanOrEqualToRightY);
}

void GameObjectLayer::AddToLayer(GameObject * pGO)
{
	m_layerObjects.push_back(pGO);
	Sorting::InsertionSort(m_layerObjects, &LeftYGreaterThanRightY);
}

void GameObjectLayer::RemoveFromLayer(GameObject * pGO)
{
	m_layerObjects.erase(
		std::remove(m_layerObjects.begin(), m_layerObjects.end(), pGO),
		m_layerObjects.end()
	);
}

void GameObjectLayer::AddLightToLayer(GameObject * pGO)
{
	int idx = m_layerLights.size() * 4;
	m_layerLights.push_back(pGO);

	LightBase* pLightComp = pGO->GetComponent<LightBase>(ComponentType::C_PointLight);

	m_lightColors[idx + 0] = pLightComp->Red();
	m_lightColors[idx + 1] = pLightComp->Green();
	m_lightColors[idx + 2] = pLightComp->Blue();
	m_lightColors[idx + 3] = pLightComp->Alpha();

	Vector3D pos = pLightComp->GetPosition();
	m_lightPositionsAndDistances[idx + 0] = pos.x;
	m_lightPositionsAndDistances[idx + 1] = pos.y;
	m_lightPositionsAndDistances[idx + 2] = pos.z;
	m_lightPositionsAndDistances[idx + 3] = pLightComp->Distance();
}

void GameObjectLayer::RemoveLightFromLayer(GameObject * pGO)
{
	m_layerLights.erase(
		std::find(m_layerLights.begin(), m_layerLights.end(), pGO),
		m_layerLights.end()
	);
}

void GameObjectLayer::Update()
{
	for (int i = 0; i < m_layerLights.size(); ++i) {
		int idx = i * 4;
		PointLight* pPointLightComp = m_layerLights[i]->GetComponent<PointLight>(ComponentType::C_PointLight);

		m_lightColors[idx + 0] = pPointLightComp->Red();
		m_lightColors[idx + 1] = pPointLightComp->Green();
		m_lightColors[idx + 2] = pPointLightComp->Blue();
		m_lightColors[idx + 3] = pPointLightComp->Alpha();

		Vector3D pos = pPointLightComp->GetPosition();
		m_lightPositionsAndDistances[idx + 0] = pos.x;
		m_lightPositionsAndDistances[idx + 1] = pos.y;
		m_lightPositionsAndDistances[idx + 2] = pos.z;
		m_lightPositionsAndDistances[idx + 3] = pPointLightComp->Distance();
	}
	m_numLights = m_layerLights.size();
}

void GameObjectLayer::ClearLayer()
{
	m_layerObjects.clear();
}

void GameObjectLayer::BindBufferDatas() const
{
	float colors[MAX_LIGHTS * 4];
	//std::fill(colors, colors + (MAX_LIGHTS * 4), 0.f);
	for (int i = 0; i < m_numLights; ++i) {
		int idx = i * 4;
		PointLight* pPointLightComp = m_layerLights[i]->GetComponent<PointLight>(ComponentType::C_PointLight);

		colors[idx + 0] = float(pPointLightComp->Red()) / 255.f;
		colors[idx + 1] = float(pPointLightComp->Green()) / 255.f;
		colors[idx + 2] = float(pPointLightComp->Blue()) / 255.f;
		colors[idx + 3] = float(pPointLightComp->Alpha()) / 255.f;
	}


	glUniform4fv(SHADER_LOCATIONS::L_COLOR, MAX_LIGHTS, colors);
	glUniform4fv(SHADER_LOCATIONS::L_POS_DIST, MAX_LIGHTS, m_lightPositionsAndDistances);
}

#pragma endregion

#pragma region GameObjectManager

GameObjectManager::GameObjectManager() : m_currentId(0) {}

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
	DestroyGameObjects();
	AddGameObjectsFromQueueToMainVector();

}

void GameObjectManager::LateUpdate(float dt) {
	for (auto gameObject : mGameObjects) {
		if (gameObject->m_isActive)
			gameObject->LateUpdate(dt);
	}

	for (unsigned int layer = 0; layer < RENDER_LAYER::L_NUM_LAYERS; ++layer) {
		m_layers[layer].ReSortLayer();
		m_layers[layer].Update();
	}
}

void GameObjectManager::UpdateForLevelEditor(float dt) {
	for (auto gameObject : mGameObjects) {
		if (gameObject->m_isActive)
			gameObject->UpdateForLevelEditor(dt);
	}
}

void GameObjectManager::LateUpdateForLevelEditor(float dt) {
	for (auto gameObject : mGameObjects) {
		if (gameObject->m_isActive)
			gameObject->LateUpdateForLevelEditor(dt);
	}

	for (unsigned int layer = 0; layer < RENDER_LAYER::L_NUM_LAYERS; ++layer) {
		m_layers[layer].ReSortLayer();
	}
}

void GameObjectManager::RenderGameObjects()
{
	// Render all layers but UI
	for (unsigned int layer = 0; layer < RENDER_LAYER::L_NUM_LAYERS - 1; ++layer) {
		for (GameObject* cameraGO : m_pCameras) {
			Camera* cameraComp = cameraGO->GetComponent<Camera>(ComponentType::C_Camera);
			if (cameraComp->ShouldRenderLayer(layer))
				m_layers[layer].RenderLayer(cameraGO);
		}
	}

	for (GameObject* cameraGO : m_pCameras) {
		if (cameraGO->GetComponent<Camera>(ComponentType::C_Camera)->ShouldRenderLayer(RENDER_LAYER::L_RENDER_DEBUG)) {
			TETRA_DEBUG.RenderDebugCommands(cameraGO);
			break;
		}
	}

	// Render UI
	for (unsigned int layer = L_UI; layer < RENDER_LAYER::L_NUM_LAYERS; ++layer) {
		for (GameObject* cameraGO : m_pCameras) {
			Camera* cameraComp = cameraGO->GetComponent<Camera>(ComponentType::C_Camera);
			if (cameraComp->ShouldRenderLayer(layer))
				m_layers[layer].RenderLayer(cameraGO);
		}
	}
}

void GameObjectManager::AddGameObject(GameObject* pGO) {
	_InsertGameObjectIntoList(pGO);

	if (pGO->HasComponent(ComponentType::C_Body)) {
		TETRA_PHYSICS.AddGameObject(pGO);
	}
	if (pGO->HasComponent(ComponentType::C_Health)) {
		mGameObjectsWithHealthComponents.push_back(pGO);
	}
	if (pGO->HasComponent(ComponentType::C_PointLight)/* or other lights here*/) {
		_InsertLightIntoLayers(pGO);
	}
}

void GameObjectManager::DestroyGameObjects() {
	for (std::vector<GameObject*>::iterator it = mGameObjects.begin(); it != mGameObjects.end();) {
		if ((*it)->m_isDestroy) {
			if ((*it)->GetLayer() != L_NOT_RENDERED)
				m_layers[(*it)->GetLayer()].RemoveFromLayer(*it);

			RemoveGameObjectsFromHealthList(*it);
			if ((*it)->m_tag == T_Camera) {
				std::vector<GameObject*>::iterator iter = std::find(m_pCameras.begin(), m_pCameras.end(), (*it));
				if (iter != m_pCameras.end())
					m_pCameras.erase(iter);
			}

			TETRA_PHYSICS.RemoveGameObject(*it);
			TETRA_MEMORY.DeleteGameObject(*it);
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

GameObject* GameObjectManager::CreateGameObject(const std::string& name) {
	json* j = TETRA_RESOURCES.GetPrefabFile(name + ".json");

	GameObject *pGameObject = TETRA_MEMORY.GetNewGameObject(TagNameText[FindTagWithString(name)], ++m_currentId);

	SetGameObjectTag(ParseString(*j, "Tag"), pGameObject);
	SetGameObjectLayer(ParseString(*j, "Layer"), pGameObject);

	// TODO: Find a cleaner way to do this?
	if (pGameObject->m_tag == GameObjectTag::T_Camera)	m_pCameras.push_back(pGameObject);
	int size = (*j)[COMPONENTS].size();
	for (int i = 0; i < size; ++i) {
		Component* pComponent = TETRA_COMPONENTS.CreateComponent(ParseString((*j)[COMPONENTS][i], "Component"));
		pGameObject->AddComponent(pComponent);
		pComponent->Serialize((*j)[COMPONENTS][i]);
	}

	pGameObject->LateInitialize();
	AddGameObjectToQueue(pGameObject);
	return pGameObject;
}

void GameObjectManager::SetGameObjectTag(std::string tag, GameObject* pGO) {
	pGO->m_tag = FindTagWithString(tag);
}

void GameObjectManager::SetGameObjectTag(GameObjectTag tag, GameObject * pGO)
{
	pGO->m_tag = tag;
}

void GameObjectManager::SetGameObjectLayer(std::string layer, GameObject * pGO)
{
	pGO->SetLayer(GetLayerFromString(layer));
}

GameObjectTag GameObjectManager::FindTagWithString(std::string tag) {
	// TODO: Convert Tags to something better, try the trick mentioned by Prof. Rabin
	return TAG_NAMES[tag];
}

void GameObjectManager::HandleEvent(Event *pEvent) {
	switch (pEvent->Type()) {
	case EventType::EVENT_OnLevelInitialized:
		UpdateStatus();
		m_pPlayerReference = FindObjectWithTag(T_Player);
		break;
	case EventType::EVENT_StaticsLoaded:
		UpdateStatus();
		break;
	}
}

void GameObjectManager::_InsertGameObjectIntoList(GameObject * pGO) {
	mGameObjects.push_back(pGO);

	if (pGO->GetLayer() != RENDER_LAYER::L_NOT_RENDERED)
		m_layers[pGO->GetLayer()].AddToLayer(pGO);
}

void GameObjectManager::_InsertLightIntoLayers(GameObject * pGO)
{
	LightBase* pLightComp = pGO->GetComponent<LightBase>(ComponentType::C_PointLight);

	for (int i = 0; i < RENDER_LAYER::L_NUM_LAYERS; ++i) {
		if (pLightComp->GetLayer(i))
			m_layers[i].AddLightToLayer(pGO);
	}
}

RENDER_LAYER GameObjectManager::GetLayerFromString(std::string layerName) {
	return RENDER_LAYER_STRINGS[layerName];
}

void GameObjectManager::RemoveGameObjectsFromHealthList(GameObject* pGO) {
	if (mGameObjectsWithHealthComponents.size() > 0) {
		std::vector<GameObject*>::iterator it = std::find(mGameObjectsWithHealthComponents.begin(), mGameObjectsWithHealthComponents.end(), pGO);
		if (it != mGameObjectsWithHealthComponents.end())
			mGameObjectsWithHealthComponents.erase(it);
	}
}

#pragma endregion
