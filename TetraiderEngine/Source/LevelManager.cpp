#include <Stdafx.h>

static const std::string GAME_OBJECTS = "GAME_OBJECTS";

LevelManager::LevelManager(): m_isRandomlyGenerated(true), m_wasRandomlyGenerated(false), m_levelsCompleted(0) {
	TETRA_EVENTS.Subscribe(EVENT_INPUT_RESTART, this);
	TETRA_EVENTS.Subscribe(EVENT_ExitLevel, this);
	TETRA_EVENTS.Subscribe(EVENT_OnLoadNextLevel, this);
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

#define LEVEL_PARAMS "LevelParameters"
void LevelManager::Initialize(const json& j) {
	levelConfig = j;
	maxLevel = levelConfig["Levels"].size();
	currentLevel = levelConfig["Start"];
	firstLevel = currentLevel;
	mainMenuLevel = levelConfig["MainMenu"];
	m_isRandomlyGenerated = ParseBool(levelConfig, "isRandomGenerated");

	std::string staticsFileName = levelConfig["Statics"];
	// TODO: Find a better spot for this? - Holden
	staticObjects = JsonReader::OpenJsonFile(TETRA_GAME_CONFIG.LevelFilesDir() + staticsFileName + ".json");

	m_levelConfigs.reserve(j[LEVEL_PARAMS].size());
	for (unsigned int i = 0; i < j[LEVEL_PARAMS].size(); ++i) {
		LevelConfig config;
		config.cols = j[LEVEL_PARAMS][i]["cols"];
		config.rows = j[LEVEL_PARAMS][i]["rows"];
		config.difficulty = j[LEVEL_PARAMS][i]["difficulty"];
		config.bossAndShop = static_cast<BossAndShop>( (j[LEVEL_PARAMS][i]["boss"] ? 1 : 0) + (j[LEVEL_PARAMS][i]["shop"] ? 2 : 0) );

		m_levelConfigs.push_back(config);
	}
	m_maxLevel = unsigned short(m_levelConfigs.size());
}
#undef LEVEL_PARAMS

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
		if (m_levelsCompleted >= (int)m_levelConfigs.size()) {
			std::cout << "Attempted to load a level out of index. Reverted to last level" << std::endl;
			m_levelsCompleted = m_levelConfigs.size() - 1;
		}

		TETRA_LEVEL_GEN.GenerateRoomNodes(m_levelConfigs[m_levelsCompleted]);
		TETRA_LEVEL_GEN.GenerateFloorPlan(m_levelConfigs[m_levelsCompleted], 1522724474);
		TETRA_LEVEL_GEN.PrintFloorPlan();
		TETRA_LEVELS.LoadStaticGameObjects();
		TETRA_LEVEL_GEN.GenerateLevelFromFloorPlan();
		m_wasRandomlyGenerated = true;
	}
	else {
		std::string s = TETRA_GAME_CONFIG.LevelFilesDir() + ParseString(levelConfig["Levels"][currentLevel], "Name") + ".json";
		_LoadLevel(OpenJsonFile(s));
		m_wasRandomlyGenerated = false;
	}

	TETRA_EVENTS.BroadcastEvent(&Event(EventType::EVENT_OnLevelInitialized));
	TETRA_PLAYERSTATS.LoadStats();
	m_isRandomlyGenerated = false;
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
		if (m_wasRandomlyGenerated)
			m_isRandomlyGenerated = true;

		TETRA_GAME_STATE.SetGameState(GameState::RESTART);
	}
	else if (i >= maxLevel) {
		printf("LEVEL DOES NOT EXIST IN CONFIG SETTINGS. LOADING LEVEL FAILED\n");
		return;
	}
	else {
		currentLevel = i;
		if (currentLevel == mainMenuLevel) {
			m_levelsCompleted = 0;
			TETRA_PLAYERSTATS.ClearStats();
		}

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
		case EVENT_INPUT_RESTART: {
			if (TETRA_GAME_STATE.m_isLevelEditorMode) return;
			InputButtonData* pButtonData = pEvent->Data<InputButtonData>();
			if (pButtonData->m_isTrigger) {
				if (m_wasRandomlyGenerated)
					m_isRandomlyGenerated = true;
				TETRA_EVENTS.BroadcastEvent(&Event(EventType::RESTART_LEVEL));
			}
			break;
		}
		case EVENT_ExitLevel: {
			m_levelsCompleted += 1;
			break;
		}
		case EVENT_OnLoadNextLevel: {
			// CURRENTLY HARDCODED NEED TO CHANGE WHEN WE HAVE ENOUGH LEVELS
			ActivateRandomGeneration(true);
			TETRA_LEVELS.ChangeLevel(2);
			break;
		}
	}
}