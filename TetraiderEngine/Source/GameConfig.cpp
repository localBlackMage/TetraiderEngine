#include "GameConfig.h"
#include "ResourceManager.h"

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
}