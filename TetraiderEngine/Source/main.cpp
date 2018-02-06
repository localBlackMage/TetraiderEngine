#include <windows.h>
#include "TetraiderAPI.h"
#include <iostream>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{
	Tetraider::Initialize("../TetraiderEngine/Assets/Metadata/Config.json");
	printf("============= Program Start ============\n\n");
	Tetraider::StartGameLoop();

	return 0;
}

