#include <windows.h>
#include "TetraiderAPI.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{
	//TODO load sound from somewhere else
	//Load audio
	TETRA_RESOURCES.LoadSong(TETRA_GAME_CONFIG.SFXDir() + "bgm.mp3");
	TETRA_RESOURCES.LoadSFX(TETRA_GAME_CONFIG.SFXDir() + "pew.mp3");

	Tetraider::Initialize("../TetraiderEngine/Assets/Metadata/Config.json");
	Tetraider::StartGameLoop();

	return 0;
}

