#include "LevelManager.h"
#include "GameStateManager.h"
#include "GameObjectManager.h"
#include "GameConfig.h"

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

void LevelManager::LoadLevel() {
	GameConfig& gameConfig = GameConfig::GetInstance();
	std::string s = gameConfig.m_levelFilesDir + ParseString(levelConfig["Levels"][currentLevel], "Name") + ".json";
	json j = OpenJsonFile(s);
	LoadLevel(j);
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
	GameStateManager& gameStateMngr = GameStateManager::GetInstance();

	if (i == currentLevel) {
		gameStateMngr.SetGameState(GameState::RESTART);
	}
	else if (i >= maxLevel) {
		printf("LEVEL DOES NOT EXIST IN CONFIG SETTINGS. LOADING LEVEL FAILED\n");
		return;
	}
	else {
		currentLevel = i;
		gameStateMngr.SetGameState(GameState::NEXT_LEVEL);
	}
}

void LevelManager::NextLevel() {
	ChangeLevel(currentLevel + 1);
}

void LevelManager::RestartGame() {
	ChangeLevel(firstLevel);
}

void LevelManager::LoadLevel(json j) {
	GameObjectManager& gameObjectManager = GameObjectManager::GetInstance();

	// TODO: Hard code this string 'GAME_OBJECTS' into a #define or something somewhere
	int gameObjectSize = j[GAME_OBJECTS].size();
	for (int i = 0; i < gameObjectSize; i++) {
		GameObject* pGO = gameObjectManager.CreateGameObject(j[GAME_OBJECTS][i]["prefab"]);

		// TODO: Clean up - Send JSON to the component itself for parsing via component.Override
		// cycle through pGO's Components, call Override on each, pass json

		// Overwrite values for transform component if they exist
		if (pGO) {
			Transform* pT = static_cast<Transform*>(pGO->GetComponent(ComponentType::Transform));
			if (pT) {
				if (j[GAME_OBJECTS][i].find("position") != j[GAME_OBJECTS][i].end()) {
					pT->SetPosition(ParseVector3D(j[GAME_OBJECTS][i], "position"));
				}
			}

			Camera* pCamera = static_cast<Camera*>(pGO->GetComponent(ComponentType::Camera));
			if (pCamera)	pCamera->Override(j[GAME_OBJECTS][i]);

			Sprite* pSprite = static_cast<Sprite*>(pGO->GetComponent(ComponentType::Sprite));
			if (pSprite)	pSprite->Override(j[GAME_OBJECTS][i]);
		}
	}

	gameObjectManager.UpdateStatus();
}