#include <Stdafx.h>

#define GAME_SETTINGS "GAME_SETTINGS"
#define RENDER_SETTINGS "RENDER_SETTINGS"
#define ROOM_SETTINGS "ROOM_SETTINGS"
#define LEVEL_SETTINGS "LEVEL_SETTINGS"
#define INPUT_SETTINGS "INPUT_SETTINGS"
#define WINDOW_SETTINGS "WINDOW_SETTINGS"
#define SHADER_LIST "SHADERS"

GameConfig::GameConfig() : m_consoleEnabled(false){}

GameConfig::~GameConfig() {}

void GameConfig::LoadConfig(std::string s) {
	json j = OpenJsonFile(s);
	json gameSettings = j[GAME_SETTINGS];
	json renderSettings = j[RENDER_SETTINGS];

	// Set project directories
	m_levelFilesDir = ParseString(gameSettings, "levelFilesDir");
	m_prefabsDir = ParseString(gameSettings, "prefabsDir");
	m_sfxDir = ParseString(gameSettings, "sfxDir");
	m_shadersDir = ParseString(gameSettings, "shadersDir");
	m_texturesDir = ParseString(gameSettings, "texturesDir");
	m_roomFilesDir = ParseString(gameSettings, "roomFilesDir");
	m_fontsDir = ParseString(gameSettings, "fontsDir");

	m_debugEnabled = ParseBool(gameSettings, "isDebugModeEnabled");

	//set mute
	m_soundsMute = ParseBool(gameSettings,"soundsMute");
	
	m_consoleEnabled = ParseBool(renderSettings, "consoleEnabled");
	if (m_consoleEnabled)
		TETRA_RENDERER.SetUpConsole();

	m_cellWidth = j[ROOM_SETTINGS]["cellWidth"];
	m_cellHeight = j[ROOM_SETTINGS]["cellHeight"];
	m_roomWidth = j[ROOM_SETTINGS]["roomWidth"];
	m_roomHeight = j[ROOM_SETTINGS]["roomHeight"];

	for (unsigned int i = 0; i < j[WINDOW_SETTINGS]["resolutions"].size(); ++i) {
		m_resolutions[i].width = j[WINDOW_SETTINGS]["resolutions"][i]["width"];
		m_resolutions[i].height = j[WINDOW_SETTINGS]["resolutions"][i]["height"];
	}
	m_currentResolution = j[WINDOW_SETTINGS]["defaultResolution"];

	TETRA_RENDERER.InitWindow(m_debugEnabled);
	TETRA_RENDERER.SetWindowDimensions(m_resolutions[m_currentResolution].width, m_resolutions[m_currentResolution].height);
	TETRA_RENDERER.SetWindowTitle(ParseString(j[WINDOW_SETTINGS], "title"));

	if (ParseBool(renderSettings, "enableWindowsCursor"))
		TETRA_RENDERER.EnableWindowsCursor();
	else
		TETRA_RENDERER.DisableWindowsCursor();

	// Set level parameters
	json levelSettings = j[LEVEL_SETTINGS];
	TETRA_LEVELS.Initialize(levelSettings);
	json inputSettings = j[INPUT_SETTINGS];
	TETRA_INPUT.Initialize(inputSettings);

	// Set debug parameters
	TETRA_DEBUG.SetDebugMode(m_debugEnabled);


	if (!TETRA_RENDERER.InitGlew())	exit(1);
	TETRA_RENDERER.SetDebugShaderName(renderSettings["debugShader"]);
	TETRA_RENDERER.LoadShaders(ParseStringList(j, SHADER_LIST));
	TETRA_RENDERER.SetGlobalAmbientLight(ParseColor(renderSettings, "globalAmbient"));

	ImageRenderersData IRD;
	IRD.pGausHShader = TETRA_RENDERER.GetShaderProgram(ParseString(renderSettings, "gaussianHorizontalShader"));
	IRD.pGausVShader = TETRA_RENDERER.GetShaderProgram(ParseString(renderSettings, "gaussianVerticalShader"));
	IRD.pBaseShader = TETRA_RENDERER.GetShaderProgram(ParseString(renderSettings, "baseShader"));
	TETRA_POST_PROCESSING.InitImageRenderers(IRD);
	m_postProcessingEnabled = ParseBool(renderSettings, "postProcessingEnabled");
	if (m_postProcessingEnabled)
		TETRA_POST_PROCESSING.Enable();
	else
		TETRA_POST_PROCESSING.Disable();



	TETRA_PLAYERSTATS.InitializePowerUps(OpenJsonFile(gameSettings["powerUpSettings"]));
}

void GameConfig::SelectResolution(unsigned short resolutionIndex)
{
	if (resolutionIndex > 3)	return;
	m_currentResolution = resolutionIndex;
	TETRA_RENDERER.SetWindowDimensions(m_resolutions[m_currentResolution].width, m_resolutions[m_currentResolution].height);

	TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EventType::WINDOW_RESIZED, &WindowResizedData(m_resolutions[m_currentResolution].width, m_resolutions[m_currentResolution].height)));
}

void GameConfig::NextResolution()
{
	if (++m_currentResolution > 3)	m_currentResolution = 0;
	SelectResolution(m_currentResolution);
}

void GameConfig::PrevResolution()
{
	if (--m_currentResolution > 3)	m_currentResolution = 3;
	SelectResolution(m_currentResolution);
}


#undef GAME_SETTINGS 
#undef RENDER_SETTINGS
#undef ROOM_SETTINGS 
#undef LEVEL_SETTINGS 
#undef INPUT_SETTINGS 
#undef WINDOW_SETTINGS
#undef SHADER_LIST