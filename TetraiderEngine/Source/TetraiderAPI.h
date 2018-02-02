#pragma once
#ifndef TETRAIDER_API_H
#define TETRAIDER_API_H

#include "GameConfig.h"
#include "GameStateManager.h"
#include "InputManager.h"
#include "FrameRateManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "PhysicsManager.h"
#include "EventManager.h"
#include "LevelManager.h"
#include "DebugManager.h"
#include "AudioManager.h"
#include "GameObjectManager.h"
#include "Singleton.h"
#include <string>

#define TETRA_GAME_CONFIG Singleton<GameConfig>::GetInstance()
#define TETRA_GAME_STATE Singleton<GameStateManager>::GetInstance()
#define TETRA_EVENTS Singleton<EventManager>::GetInstance()
#define TETRA_FRAMERATE Singleton<FrameRateManager>::GetInstance()
#define TETRA_RENDERER Singleton<RenderManager>::GetInstance()
#define TETRA_INPUT Singleton<InputManager>::GetInstance()
#define TETRA_RESOURCES Singleton<ResourceManager>::GetInstance()
#define TETRA_LEVELS Singleton<LevelManager>::GetInstance()
#define TETRA_DEBUG Singleton<DebugManager>::GetInstance()
#define TETRA_AUDIO Singleton<AudioManager>::GetInstance()
#define TETRA_PHYSICS Singleton<PhysicsManager>::GetInstance()
#define TETRA_GAME_OBJECTS Singleton<GameObjectManager>::GetInstance()


namespace Tetraider {
	//! int Initialize(std::string configFileName)
	/*!
	\param configFileName The config json file to use for setup, must include file path
	\return Error code: 0 - everything went well, non-zero - something went wrong
	*/
	int Initialize(std::string configFileName);

	void StartGameLoop();

	float GetFrameTime();

	//! double FrameStart()
	/*!
	Starts the game frame. Calls various start and update methods on managers.
	*/
	void FrameStart();

	//! void Update(double deltaTime)
	/*!
	Calls various update methods on managers
	\param deltaTime - Duration of the previous frame
	*/
	void Update(float deltaTime);

	//! void FrameEnd()
	/*!
	Ends the game frame. Calls various end and update methods on managers.
	*/
	void FrameEnd();

	//! Mesh * CreateMesh(std::string meshName)
	/*!
	Creates a Mesh object and maps it to the given name
	\param meshName - std::string name representing the Mesh to be created, used for resource management
	\return pointer to the mesh created
	*/
	Mesh * CreateMesh(std::string meshName);

	//! void UnloadResources()
	/*!
	Calls to unload all currently loaded resources. Meshes, textures, game object archetypes, etc.
	*/
	void UnloadResources();

	//! void LoadGameObjects(std::string fileName)
	/*!
	Parses a given json file which in turn should list other json files each containing a game object archetype.
	Each archetype is loaded into the engine for use in creation later.
	\param fileName - The json file containing what game object archetypess to load, must include file path
	*/
	void LoadPrefabs(std::string filePath);
}

#endif
