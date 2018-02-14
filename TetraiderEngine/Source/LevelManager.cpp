#include "GameObject.h"
#include "LevelManager.h"
#include "TetraiderAPI.h"

#include "Transform.h"
#include "Body.h"
#include "Shape.h"
#include <iostream>

static const std::string GAME_OBJECTS = "GAME_OBJECTS";

LevelManager::LevelManager(): m_isRandomlyGenerated(true) {}

LevelManager::~LevelManager() {}

bool LevelManager::IsLastLevel() {return currentLevel == (maxLevel - 1); }

void LevelManager::LoadStaticGameObjects()
{
	int gameObjectSize = staticObjects[GAME_OBJECTS].size();
	for (int i = 0; i < gameObjectSize; i++) {
		TETRA_GAME_OBJECTS.CreateGameObject(staticObjects[GAME_OBJECTS][i]["prefab"]);
	}

	TETRA_EVENTS.BroadcastEvent(&Event(EventType::EVENT_StaticsLoaded));
}

void LevelManager::Initialize(const json& j) {
	levelConfig = j;
	maxLevel = levelConfig["Levels"].size();
	currentLevel = levelConfig["Start"];
	firstLevel = currentLevel;
	m_isRandomlyGenerated = ParseBool(levelConfig, "isRandomGenerated");

	std::string staticsFileName = levelConfig["Statics"];
	// TODO: Find a better spot for this? - Holden
	staticObjects = JsonReader::OpenJsonFile(TETRA_GAME_CONFIG.LevelFilesDir() + staticsFileName + ".json");
}

std::vector<GameObject*> LevelManager::LoadRoomFile(const json & j)
{
	std::vector<GameObject*> createdGameObjects;
	int gameObjectSize = j[GAME_OBJECTS].size();
	for (int i = 0; i < gameObjectSize; i++) {
		GameObject* pGO = TETRA_GAME_OBJECTS.CreateGameObject(j[GAME_OBJECTS][i]["prefab"]);

		if (pGO)
			pGO->OverrideComponents(j[GAME_OBJECTS][i]);
		createdGameObjects.push_back(pGO);
	}

	return createdGameObjects;
}

void LevelManager::LoadLevel() {
	//if(levelConfig.)
	if (m_isRandomlyGenerated) {
		TETRA_LEVEL_GEN.GenerateFloorPlan();
		TETRA_LEVEL_GEN.PrintFloorPlan();
		TETRA_LEVELS.LoadStaticGameObjects();
		TETRA_LEVEL_GEN.GenerateLevelFromFloorPlan();
	}
	else {
		std::string s = TETRA_GAME_CONFIG.LevelFilesDir() + ParseString(levelConfig["Levels"][currentLevel], "Name") + ".json";
		_LoadLevel(OpenJsonFile(s));
	}
}

void LevelManager::UnLoadLevel() {
	UnLoadLevelForRestart();
	// TODO: Unload any other assets required before loading next level
}

void LevelManager::UnLoadLevelForRestart() {
	TETRA_DEBUG.ClearDebugCommands();
	TETRA_GAME_OBJECTS.DestroyAllGameObjects();
	TETRA_EVENTS.ClearDelayedEvents();
}

void LevelManager::ChangeLevel(int i) {
	if (i == currentLevel) {
		TETRA_GAME_STATE.SetGameState(GameState::RESTART);
	}
	else if (i >= maxLevel) {
		printf("LEVEL DOES NOT EXIST IN CONFIG SETTINGS. LOADING LEVEL FAILED\n");
		return;
	}
	else {
		currentLevel = i;
		TETRA_GAME_STATE.SetGameState(GameState::NEXT_LEVEL);
	}
}

void LevelManager::NextLevel() {
	ChangeLevel(currentLevel + 1);
}

void LevelManager::RestartGame() {
	ChangeLevel(firstLevel);
}

void LevelManager::_LoadLevel(const json& j) {
	int gameObjectSize = j[GAME_OBJECTS].size();
	for (int i = 0; i < gameObjectSize; i++) {
		GameObject* pGO = TETRA_GAME_OBJECTS.CreateGameObject(j[GAME_OBJECTS][i]["prefab"]);

		// Overwrite values for transform component if they exist
		// TODO: Scale and rotation as well
		if (pGO) {
			pGO->OverrideComponents(j[GAME_OBJECTS][i]);

			//Transform* pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
			//if (pTransform)	pTransform->Override(j[GAME_OBJECTS][i]);

			//Body* pBody = pGO->GetComponent<Body>(ComponentType::C_Body);
			//if (pBody)	pBody->Override(j[GAME_OBJECTS][i]);
		}
	}

	TETRA_EVENTS.BroadcastEvent(&Event(EventType::EVENT_OnLevelInitialized));
}