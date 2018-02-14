#include "TetraiderAPI.h"

namespace Tetraider {

	int Initialize(std::string configFile)
	{
		
		TETRA_GAME_CONFIG.LoadConfig(configFile);
		TETRA_RENDERER.Init();
		TETRA_RENDERER.LoadShaders();
		TETRA_RESOURCES.Init();

		TETRA_LEVEL_GEN.LoadRoomFiles();

		return 0;
	}

	void StartGameLoop()
	{
		TETRA_GAME_STATE.Update(); // Start game loop
	}

	float GetFrameTime()
	{
		return TETRA_FRAMERATE.GetFrameTime();;
	}

	void FrameStart()
	{
		TETRA_FRAMERATE.FrameStart();
		TETRA_RENDERER.FrameStart();
	}

	void Update(float deltaTime)
	{
		TETRA_INPUT.Update();									// Update input keys
		TETRA_DEBUG.Update();									// Toggles debug drawing if needed
		TETRA_EVENTS.Update(deltaTime);							// Pump the event manager
		//TETRA_AUDIO.Update(deltaTime);
		TETRA_GAME_OBJECTS.Update(deltaTime);					// Update game logic
		TETRA_GAME_OBJECTS.UpdateStatus();						// Update status of game objects
		TETRA_PHYSICS.Integrate(deltaTime);						// Move physics bodies
		TETRA_PHYSICS.ResolveCollisions();						// Resolve collisions on physics bodies
		TETRA_GAME_OBJECTS.LateUpdate(deltaTime);				// Update game logic that occurs after physics
		TETRA_GAME_OBJECTS.RenderGameObjects();					// Render all game objects
		TETRA_DEBUG.RenderDebugCommands();
	}

	void FrameEnd()
	{
		TETRA_RENDERER.FrameEnd();								// Swap window buffer
		TETRA_FRAMERATE.FrameEnd();								// Lock FPS 
		// TODO: Clean up GameObjects
	}

	void DebugMode()
	{
		while (true) {
			TETRA_INPUT.Update();								// Update input keys
			// Step in one frame
			if (TETRA_INPUT.IsKeyTriggered(SDL_SCANCODE_PERIOD)) {
				Update(TETRA_FRAMERATE.GetMaxFrameRate());
				break;
			}
			else if (TETRA_INPUT.IsKeyPressed(SDL_SCANCODE_SLASH)) {
				Update(TETRA_FRAMERATE.GetMaxFrameRate());
				break;
			}
			// Exit debug mode
			else if (TETRA_INPUT.IsKeyTriggered(SDL_SCANCODE_F2))
				break;
		}
	}

	Mesh * CreateMesh(std::string meshName)
	{
		return TETRA_RESOURCES.LoadMesh(meshName);
	}

	void UnloadResources()
	{
		TETRA_RESOURCES.UnloadAll();
	}

	void LoadPrefabs(std::string fileName)
	{
		//T_GO_FACTORY.LoadGameObjectsFromFile(fileName);
	}
}
