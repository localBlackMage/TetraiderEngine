#include "LevelManager.h"
#include "GameStateManager.h"
#include "GameObjectManager.h"
#include "GameConfig.h"
#include "Transform.h"

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

	int gameObjectSize = j["GAME_OBJECTS"].size();
	for (int i = 0; i < gameObjectSize; i++) {
		GameObject* pGO = gameObjectManager.CreateGameObject(j["GAME_OBJECTS"][i]["prefab"]);

		// TODO: Clean up
		// Overwrite values for transform component if they exist
		if (pGO) {
			Transform* pT = static_cast<Transform*>(pGO->GetComponent(ComponentType::Transform));
			if (pT) {
				if (j["GAME_OBJECTS"][i].find("positionX") != j["GAME_OBJECTS"][i].end() && j["GAME_OBJECTS"][i].find("positionY") != j["GAME_OBJECTS"][i].end() && j["GAME_OBJECTS"][i].find("positionZ") != j["GAME_OBJECTS"][i].end()) {
					Vector3D pos;
					pos.Set(ParseFloat(j["GAME_OBJECTS"][i], "positionX"), ParseFloat(j["GAME_OBJECTS"][i], "positionY"), ParseFloat(j["GAME_OBJECTS"][i], "positionZ"));
					pT->SetPosition(&pos);
				}
				if (j["GAME_OBJECTS"][i].find("scaleX") != j["GAME_OBJECTS"][i].end())
					pT->m_pScale->x = j["GAME_OBJECTS"][i]["scaleX"];
				if (j["GAME_OBJECTS"][i].find("scaleY") != j["GAME_OBJECTS"][i].end())
					pT->m_pScale->y = j["GAME_OBJECTS"][i]["scaleY"];
				if (j["GAME_OBJECTS"][i].find("scaleZ") != j["GAME_OBJECTS"][i].end())
					pT->m_pScale->z = j["GAME_OBJECTS"][i]["scaleZ"];
				if (j["GAME_OBJECTS"][i].find("rotation") != j["GAME_OBJECTS"][i].end())
					pT->m_angle = j["GAME_OBJECTS"][i]["rotation"];
			}
		}
	}

	gameObjectManager.UpdateStatus();
}