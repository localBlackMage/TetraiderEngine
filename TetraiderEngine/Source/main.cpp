#include <windows.h>
#include "RenderManager.h"
#include "GameConfig.h"
#include "GameStateManager.h"
#include "AudioManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{
	bool done = false;
	double dt = 0.0;

	// TODO: Cleanup
	GameConfig& gameConfig = GameConfig::GetInstance();
	gameConfig.LoadConfig("../TetraiderEngine/Assets/Metadata/Config.json");

	//init audio
	AudioManager& audio = AudioManager::GetInstance();

	//TODO load sound from somewhere else
	//Load audio
	audio.LoadSong("../TetraiderEngine/Assets/SFX/bgm.mp3");
	audio.LoadSFX("../TetraiderEngine/Assets/SFX/pew.mp3");

	RenderManager& renderMngr = RenderManager::GetInstance();
	renderMngr.Init();
	renderMngr.LoadShaders();
	// TODO: Load shader from somewhere else
	//renderMngr.LoadShaderProgram("../TetraiderEngine/Assets/Shaders/defaultShader.json");


	GameStateManager& gameStateMngr = GameStateManager::GetInstance();
	gameStateMngr.Update(); // Start game loop

	return 0;
}

