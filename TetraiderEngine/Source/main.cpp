#include <windows.h>
#include "FrameRateManager.h"
#include "ResourceManager.h"
#include "RenderManager.h"

// TODO: Remove
#include "STBSurface.h"
#include "Mesh.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{
	bool done = false;
	double dt = 0.0;

	// TODO: Cleanup
	FrameRateManager& frameRateMngr = FrameRateManager::GetInstance();
	RenderManager& renderMngr = RenderManager::GetInstance();
	renderMngr.Init();
	ResourceManager& resourceMngr = ResourceManager::GetInstance();
	
	// TODO: Remove
	//TextureInfo info;
	//info.frameWidth = 64;
	//info.frameHeight = 64;
	//info.rows = 0;
	//info.cols = 0;
	//info.hasAlpha = false;
	//SurfaceTextureBuffer * pSTB = resourceMngr.LoadTexture("Test", "../TetraiderEngine/Assets/Textures/test.png", info);
	Mesh * pMesh = resourceMngr.GetMesh("quad");

	renderMngr.LoadShaderProgram("../TetraiderEngine/Assets/Shaders/defaultShader.json");

	while (!done) {
		frameRateMngr.FrameStart();
		dt = frameRateMngr.GetFrameTime();
		renderMngr.FrameStart();

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				break;
			}
		}

		renderMngr.RenderSTB(nullptr, pMesh);

		
		renderMngr.FrameEnd();
		frameRateMngr.FrameEnd();
	}

	return 0;
}

