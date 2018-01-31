#include "GameConfig.h"
#include "TetraiderAPI.h"

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
	T_LEVELS.Initialize(levelSettings);

	// Set debug parameters
	T_DEBUG.SetDebugMode(ParseBool(gameSettings, "isDebugModeEnabled"));

	T_RENDERER.SetDebugShaderName(ParseString(gameSettings, "debugShader"));
}