#include <Stdafx.h>

#define GAME_SETTINGS "GAME_SETTINGS"
#define ROOM_SETTINGS "ROOM_SETTINGS"
#define LEVEL_SETTINGS "LEVEL_SETTINGS"
#define INPUT_SETTINGS "INPUT_SETTINGS"
#define WINDOW_SETTINGS "WINDOW_SETTINGS"

GameConfig::GameConfig() : m_consoleEnabled(false){}

GameConfig::~GameConfig() {}

void GameConfig::LoadConfig(std::string s) {
	json j = OpenJsonFile(s);
	json gameSettings = j[GAME_SETTINGS];

	// Set project directories
	m_levelFilesDir = ParseString(gameSettings, "levelFilesDir");
	m_prefabsDir = ParseString(gameSettings, "prefabsDir");
	m_sfxDir = ParseString(gameSettings, "sfxDir");
	m_shadersDir = ParseString(gameSettings, "shadersDir");
	m_texturesDir = ParseString(gameSettings, "texturesDir");
	m_roomFilesDir = ParseString(gameSettings, "roomFilesDir");
	m_fontsDir = ParseString(gameSettings, "fontsDir");

	m_debugEnabled = ParseBool(gameSettings, "isDebugModeEnabled");

	if (ParseBool(gameSettings, "enableWindowsCursor"))
		TETRA_RENDERER.EnableWindowsCursor();
	else
		TETRA_RENDERER.DisableWindowsCursor();

	//set mute
	m_soundsMute = ParseBool(gameSettings,"soundsMute");
	
	m_consoleEnabled = ParseBool(gameSettings, "consoleEnabled");
	if (m_consoleEnabled)
		TETRA_RENDERER.SetUpConsole();

	m_cellWidth = j[ROOM_SETTINGS]["cellWidth"];
	m_cellHeight = j[ROOM_SETTINGS]["cellHeight"];
	m_roomWidth = j[ROOM_SETTINGS]["roomWidth"];
	m_roomHeight = j[ROOM_SETTINGS]["roomHeight"];

	m_screenWidth = j[WINDOW_SETTINGS]["width"];
	m_screenHeight = j[WINDOW_SETTINGS]["height"];
	TETRA_RENDERER.InitWindow(m_debugEnabled);
	TETRA_RENDERER.SetWindowDimensions(m_screenWidth, m_screenHeight);
	TETRA_RENDERER.SetWindowTitle(ParseString(j[WINDOW_SETTINGS], "title"));

	// Set level parameters
	json levelSettings = j[LEVEL_SETTINGS];
	TETRA_LEVELS.Initialize(levelSettings);
	json inputSettings = j[INPUT_SETTINGS];
	TETRA_INPUT.Initialize(inputSettings);

	// Set debug parameters
	TETRA_DEBUG.SetDebugMode(m_debugEnabled);

	TETRA_RENDERER.SetDebugShaderName(gameSettings["debugShader"]);
	TETRA_RENDERER.SetGlobalAmbientLight(ParseColor(gameSettings, "globalAmbient"));

	TETRA_PLAYERSTATS.InitializePowerUps(OpenJsonFile(gameSettings["powerUpSettings"]));
}


#undef GAME_SETTINGS 
#undef ROOM_SETTINGS 
#undef LEVEL_SETTINGS 
#undef INPUT_SETTINGS 
#undef WINDOW_SETTINGS