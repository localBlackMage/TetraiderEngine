#include "TetraiderAPI.h"

namespace Tetraider {

	int Initialize(std::string configFile)
	{
		
		T_GAME_CONFIG.LoadConfig(configFile);
		T_RENDERER.Init();
		T_RENDERER.LoadShaders();
		T_RESOURCES.Init();

		return 0;
	}

	void StartGameLoop()
	{
		T_GAME_STATE.Update(); // Start game loop
	}

	float GetFrameTime()
	{
		return T_FRAMERATE.GetFrameTime();;
	}

	void FrameStart()
	{
		T_FRAMERATE.FrameStart();
		T_RENDERER.FrameStart();
	}

	void Update(float deltaTime)
	{
		T_INPUT.Update();									// Update input keys
		T_EVENTS.Update(deltaTime);							// Pump the event manager
		T_AUDIO.Update(deltaTime);
		T_GAME_OBJECTS.Update(deltaTime);					// Update game logic
		T_GAME_OBJECTS.UpdateStatus();						// Update status of game objects
		T_PHYSICS.Integrate(deltaTime);						// Move physics bodies
		T_PHYSICS.ResolveCollisions();						// Resolve collisions on physics bodies
		T_GAME_OBJECTS.LateUpdate(deltaTime);				// Update game logic that occurs after physics
		T_GAME_OBJECTS.RenderGameObjects();					// Render all game objects
		T_DEBUG.RenderDebugCommands(*T_GAME_OBJECTS.GetActiveCamera());
	}

	void FrameEnd()
	{
		T_RENDERER.FrameEnd();								// Swap window buffer
		T_FRAMERATE.FrameEnd();								// Lock FPS 
		// TODO: Clean up GameObjects
	}

	Mesh * CreateMesh(std::string meshName)
	{
		return T_RESOURCES.LoadMesh(meshName);
	}

	void UnloadResources()
	{
		T_RESOURCES.UnloadAll();
	}

	void LoadPrefabs(std::string fileName)
	{
		//T_GO_FACTORY.LoadGameObjectsFromFile(fileName);
	}
}
