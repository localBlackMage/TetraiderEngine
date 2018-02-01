#include <windows.h>
#include "TetraiderAPI.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{
	//TODO load sound from somewhere else
	//Load audio
	T_AUDIO.LoadSong("../TetraiderEngine/Assets/SFX/bgm.mp3");
	T_AUDIO.LoadSFX("../TetraiderEngine/Assets/SFX/pew.mp3");


	Tetraider::Initialize("../TetraiderEngine/Assets/Metadata/Config.json");
	Tetraider::StartGameLoop();

	return 0;
}

