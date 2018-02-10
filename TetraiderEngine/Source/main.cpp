#include <windows.h>
#include "TetraiderAPI.h"
#include <iostream>
#include "FloorPlanGenerator.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{
	//FloorPlanGenerator fpg;
	//fpg.GenerateFloorPlan();
	//fpg.PrintFloorPlan();

	Tetraider::Initialize("../TetraiderEngine/Assets/Metadata/Config.json");
	Tetraider::StartGameLoop();

	return 0;
}

