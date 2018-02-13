#include "GameObject.h"
#include "LevelManager.h"
#include "TetraiderAPI.h"

#include "Transform.h"
#include "Body.h"
#include "Shape.h"
#include <iostream>

static const std::string GAME_OBJECTS = "GAME_OBJECTS";

LevelManager::LevelManager() {}

LevelManager::~LevelManager() {}

bool LevelManager::IsLastLevel() {return currentLevel == (maxLevel - 1); }

void LevelManager::Initialize(const json& j) {
	levelConfig = j;
	maxLevel = levelConfig["Levels"].size();
	currentLevel = levelConfig["Start"];
	firstLevel = currentLevel;
}

std::vector<GameObject*> LevelManager::LoadRoomFile(const json & j)
{
	std::vector<GameObject*> createdGameObjects;
	int gameObjectSize = j[GAME_OBJECTS].size();
	for (int i = 0; i < gameObjectSize; i++) {
		GameObject* pGO = TETRA_GAME_OBJECTS.CreateGameObject(j[GAME_OBJECTS][i]["prefab"]);

		if (pGO) {
			if (j[GAME_OBJECTS][i].find("position") != j[GAME_OBJECTS][i].end()) {
				Transform* pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
				if (pTransform) pTransform->SetPosition(ParseVector3D(j[GAME_OBJECTS][i], "position"));
			}
		}
		createdGameObjects.push_back(pGO);
	}

	return createdGameObjects;
}

void LevelManager::LoadLevel() {
	std::string s = TETRA_GAME_CONFIG.LevelFilesDir() + ParseString(levelConfig["Levels"][currentLevel], "Name") + ".json";
	LoadLevel(OpenJsonFile(s));
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

void LevelManager::LoadLevel(const json& j) {
	int gameObjectSize = j[GAME_OBJECTS].size();
	for (int i = 0; i < gameObjectSize; i++) {
		GameObject* pGO = TETRA_GAME_OBJECTS.CreateGameObject(j[GAME_OBJECTS][i]["prefab"]);

		// Overwrite values for transform component if they exist
		// TODO: Scale and rotation as well
		if (pGO) {
			Transform* pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
			if (j["GAME_OBJECTS"][i].find("position") != j["GAME_OBJECTS"][i].end()) {
				if (pTransform) pTransform->SetPosition(ParseVector3D(j["GAME_OBJECTS"][i], "position"));
			}
			if (j["GAME_OBJECTS"][i].find("scale") != j["GAME_OBJECTS"][i].end()) {
				if (pTransform) pTransform->SetScale(ParseFloat(j["GAME_OBJECTS"][i]["scale"], "x"), ParseFloat(j["GAME_OBJECTS"][i]["scale"], "y"));
			}
			if (j["GAME_OBJECTS"][i].find("shape") != j["GAME_OBJECTS"][i].end()) {
				Body* pBody = pGO->GetComponent<Body>(ComponentType::C_Body);

				if (pBody->m_pShape->type == ShapeType::ST_AABB) {
					pBody->OverrideShapeData(ParseFloat(j["GAME_OBJECTS"][i]["shape"], "width"), ParseFloat(j["GAME_OBJECTS"][i]["shape"], "height"));
				}
				else if (pBody->m_pShape->type == ShapeType::ST_Circle) {
					pBody->OverrideShapeData(ParseFloat(j["GAME_OBJECTS"][i]["shape"], "radius"));
				}
			}
		}
	}

	TETRA_EVENTS.BroadcastEvent(&Event(EventType::EVENT_OnLevelInitialized));
}