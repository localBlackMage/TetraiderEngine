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
	m_roomFilesDir = ParseString(gameSettings, "roomFilesDir");
	if (ParseBool(gameSettings, "enableWindowsCursor"))
		TETRA_RENDERER.EnableWindowsCursor();
	else
		TETRA_RENDERER.DisableWindowsCursor();

	//set mute
	m_soundsMute = ParseBool(gameSettings,"soundsMute");

	m_cellWidth = j["ROOM_SETTINGS"]["cellWidth"];
	m_cellHeight = j["ROOM_SETTINGS"]["cellHeight"];
	m_roomWidth = j["ROOM_SETTINGS"]["roomWidth"];
	m_roomHeight = j["ROOM_SETTINGS"]["roomHeight"];

	// Set level parameters
	json levelSettings = j["LEVEL_SETTINGS"];
	TETRA_LEVELS.Initialize(levelSettings);
	json inputSettings = j["INPUT_SETTINGS"];
	TETRA_INPUT.Initialize(inputSettings);

	// Set debug parameters
	TETRA_DEBUG.SetDebugMode(ParseBool(gameSettings, "isDebugModeEnabled"));

	TETRA_RENDERER.SetDebugShaderName(gameSettings["debugShader"]);
}