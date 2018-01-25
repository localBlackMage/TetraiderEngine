#include "TetraiderAPI.h"
//#include "WindowManager.h"
#include "InputManager.h"
#include "FrameRateManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "GameObjectManager.h"
//#include "GameObjectFactory.h"
#include "PhysicsManager.h"
#include "EventManager.h"
#include "LevelManager.h"
#include "DebugManager.h"
//#include "json.hpp"
#include <iostream>
#include "JsonReader.h"
#include "Mesh.h"

static EventManager& eventMngr = EventManager::GetInstance();
static FrameRateManager& frameRateMngr = FrameRateManager::GetInstance();
//static WindowManager& windowMngr = WindowManager::GetInstance();
static RenderManager& renderMngr = RenderManager::GetInstance();
static InputManager& inputMngr = InputManager::GetInstance();
static ResourceManager& resourceMngr = ResourceManager::GetInstance();
//static GameObjectFactory& gameObjectFactory = GameObjectFactory::GetInstance();
static GameObjectManager& gameObjectMngr = GameObjectManager::GetInstance();
static PhysicsManager& physicsMngr = PhysicsManager::GetInstance();
static LevelManager& levelMngr = LevelManager::GetInstance();
static DebugManager& debugMngr = DebugManager::GetInstance();


namespace Tetraider {

	int Initialize(std::string configFileName)
	{
		return 0;
	}

	double GetFrameTime()
	{
		return frameRateMngr.GetFrameTime();;
	}

	void FrameStart()
	{
		frameRateMngr.FrameStart();
		renderMngr.FrameStart();
	}

	void Update(double deltaTime)
	{
		inputMngr.Update();									// Update input keys
		eventMngr.Update(deltaTime);
		gameObjectMngr.Update(deltaTime);					// Update game logic
		gameObjectMngr.UpdateStatus();						// Update status of game objects
		physicsMngr.Integrate(deltaTime);					// Move physics bodies
		physicsMngr.ResolveCollisions();					// Resolve collisions on physics bodies
		gameObjectMngr.LateUpdate(deltaTime);				// Update game logic that occurs after physics
		gameObjectMngr.RenderGameObjects();					// Render all game objects
		debugMngr.RenderDebugCommands(*gameObjectMngr.GetActiveCamera());
	}

	void FrameEnd()
	{
		renderMngr.FrameEnd();								// Swap window buffer
		frameRateMngr.FrameEnd();							// Lock FPS 
		// TODO: Clean up GameObjects
	}

	Mesh * CreateMesh(std::string meshName)
	{
		return resourceMngr.LoadMesh(meshName);
	}

	void UnloadResources()
	{
		resourceMngr.UnloadAll();
	}

	void LoadPrefabs(std::string fileName)
	{
		//gameObjectFactory.LoadGameObjectsFromFile(fileName);
	}
}
