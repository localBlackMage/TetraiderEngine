#include <Stdafx.h>

std::string ExePath() {
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
}

int Mode() {
	std::string path = ExePath();
	if (path.find("Debug") || path.find("Release"))	
		return 0;
	else											
		return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{
	std::string configFilePath = "config.json";

	TETRA_EVENTS.Init();
	Tetraider::Initialize(configFilePath);
	Tetraider::StartGameLoop();

	return 0;
}

