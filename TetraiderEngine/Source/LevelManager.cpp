#include "GameObject.h"
#include "LevelManager.h"
#include "TetraiderAPI.h"
#include "Transform.h"
#include "Body.h"
#include "Shape.h"
#include <iostream>

static const std::string GAME_OBJECTS = "GAME_OBJECTS";

LevelManager::LevelManager(): m_isRandomlyGenerated(true) {
	TETRA_EVENTS.Subscribe(EVENT_INPUT_RESTART, this);
}

LevelManager::~LevelManager() {}

bool LevelManager::IsLastLevel() {return currentLevel == (maxLevel - 1); }

int LevelManager::GetCurrentLevel()
{
	return currentLevel;
}

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

	TETRA_EVENTS.BroadcastEvent(&Event(EventType::EVENT_OnLevelInitialized));
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

	TETRA_GAME_STATE.m_isLevelEditorMode = false;
}

void LevelManager::LoadLevelEditor(int i) {
	ChangeLevel(i);
	TETRA_GAME_STATE.m_isLevelEditorMode = true;
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
		if (pGO) {
			pGO->OverrideComponents(j[GAME_OBJECTS][i]);
		}
	}
}

void LevelManager::HandleEvent(Event* pEvent) {
	switch (pEvent->Type()) {
		case EVENT_INPUT_RESTART:
			InputButtonData* pButtonData = pEvent->Data<InputButtonData>();
			if (pButtonData->m_isTrigger) TETRA_EVENTS.BroadcastEvent(&Event(EventType::RESTART_LEVEL));
			break;
	}
}