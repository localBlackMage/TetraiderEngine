#include "GameObject.h"
#include "LevelManager.h"
#include "TetraiderAPI.h"

#include "Transform.h"
#include "Camera.h"
#include "Sprite.h"

static const std::string GAME_OBJECTS = "GAME_OBJECTS";

LevelManager::LevelManager() {}

LevelManager::~LevelManager() {}

bool LevelManager::IsLastLevel() {return currentLevel == (maxLevel - 1); }

void LevelManager::Initialize(json j) {
	levelConfig = j;
	maxLevel = levelConfig["Levels"].size();
	currentLevel = levelConfig["Start"];
	firstLevel = currentLevel;
}

#include <iostream>
void LevelManager::LoadLevel() {
	std::string s = T_GAME_CONFIG.LevelFilesDir() + ParseString(levelConfig["Levels"][currentLevel], "Name") + ".json";
	LoadLevel(OpenJsonFile(s));
}

void LevelManager::UnLoadLevel() {
	UnLoadLevelForRestart();
	// TODO: Unload any other assets required before loading next level
}

void LevelManager::UnLoadLevelForRestart() {
	// TODO: Unload all assets required for level restart
	/*mainManager.pDebugManager->ClearDebugCommands();
	mainManager.pGameObjectManager->DestroyAllGameObjects();
	mainManager.pEventManager->ClearTimedEvents();
	mainManager.pEnemyManager->Reset();*/
}

void LevelManager::ChangeLevel(int i) {
	if (i == currentLevel) {
		T_GAME_STATE.SetGameState(GameState::RESTART);
	}
	else if (i >= maxLevel) {
		printf("LEVEL DOES NOT EXIST IN CONFIG SETTINGS. LOADING LEVEL FAILED\n");
		return;
	}
	else {
		currentLevel = i;
		T_GAME_STATE.SetGameState(GameState::NEXT_LEVEL);
	}
}

void LevelManager::NextLevel() {
	ChangeLevel(currentLevel + 1);
}

void LevelManager::RestartGame() {
	ChangeLevel(firstLevel);
}

void LevelManager::LoadLevel(json j) {
	// TODO: Hard code this string 'GAME_OBJECTS' into a #define or something somewhere
	int gameObjectSize = j[GAME_OBJECTS].size();
	for (int i = 0; i < gameObjectSize; i++) {
		GameObject* pGO = T_GAME_OBJECTS.CreateGameObject(j[GAME_OBJECTS][i]["prefab"]);

		// TODO: Clean up - Send JSON to the component itself for parsing via component.Override
		// cycle through pGO's Components, call Override on each, pass json

		// Overwrite values for transform component if they exist
		if (pGO) {
			if (j["GAME_OBJECTS"][i].find("position") != j["GAME_OBJECTS"][i].end()) {
				Transform* pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
				if (pTransform) pTransform->SetPosition(ParseVector3D(j["GAME_OBJECTS"][i], "position"));
			}

			//	Transform* pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
			//	if (pTransform)	pTransform->Serialize(j[GAME_OBJECTS][i]);

			//	Camera* pCamera = pGO->GetComponent<Camera>(ComponentType::C_Camera);
			//	if (pCamera)	pCamera->Serialize(j[GAME_OBJECTS][i]);

			//	Sprite* pSprite = pGO->GetComponent<Sprite>(ComponentType::C_Sprite);
			//	if (pSprite)	pSprite->Serialize(j[GAME_OBJECTS][i]);
		}
	}

	OnLevelInitialized onLevelInitialized;
	T_EVENTS.BroadcastEvent(&onLevelInitialized);
}