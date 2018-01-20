#include "GameConfig.h"
#include "LevelManager.h"
#include "DebugManager.h"

using namespace JsonReader;

GameConfig::GameConfig() {}

GameConfig::~GameConfig() {}

void GameConfig::LoadConfig(std::string s) {
	//return;
	json j = OpenJsonFile(s);
	json gameSettings = j["GAME_SETTINGS"];

	// Set project directories
	m_levelFilesDir = ParseString(gameSettings, "levelFilesDir");
	m_prefabsDir = ParseString(gameSettings, "prefabsDir");
	m_sfxDir = ParseString(gameSettings, "sfxDir");
	m_shadersDir = ParseString(gameSettings, "shadersDir");
	m_texturesDir = ParseString(gameSettings, "texturesDir");

	// Set level parameters
	json levelSettings = j["LEVEL_SETTINGS"];
	LevelManager& levelManager = LevelManager::GetInstance();
	levelManager.Initialize(levelSettings);

	// Set debug parameters
	DebugManager& debugManager = DebugManager::GetInstance();
	debugManager.m_isDebugModeEnabled = ParseBool(gameSettings, "isDebugModeEnabled");
}