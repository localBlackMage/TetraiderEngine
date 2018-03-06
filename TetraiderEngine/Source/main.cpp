//#include <windows.h>
//#include "TetraiderAPI.h"
//#include <iostream>

#include <Stdafx.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{
	Tetraider::Initialize("../TetraiderEngine/Assets/Metadata/Config.json");
	Tetraider::StartGameLoop();

	return 0;
}

