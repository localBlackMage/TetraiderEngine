#include <windows.h>
#include "TetraiderAPI.h"
#include <iostream>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{
	TETRA_LEVEL_GEN.GenerateFloorPlan();
	TETRA_LEVEL_GEN.PrintFloorPlan();

	//Tetraider::Initialize("../TetraiderEngine/Assets/Metadata/Config.json");
	//Tetraider::StartGameLoop();

	return 0;
}

