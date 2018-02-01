#include "GameStateManager.h"
#include "FrameRateManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "GameObjectManager.h"
#include "InputManager.h"
#include "LevelManager.h"
#include "PhysicsManager.h"
#include "DebugManager.h"
#include "AudioManager.h"

GameStateManager::GameStateManager(): m_previousState(GameState::CURRENT_LEVEL), m_currentState(GameState::CURRENT_LEVEL), m_nextState(GameState::CURRENT_LEVEL) {}

GameStateManager::~GameStateManager() {}

void GameStateManager::Update() {
	LevelManager& levelMngr = LevelManager::GetInstance();
	AudioManager& audio = AudioManager::GetInstance();
	SDL_Event event;

	//start back ground music
	audio.PlaySong("../TetraiderEngine/Assets/SFX/bgm.mp3");

	while (m_currentState != GameState::QUIT) {
		levelMngr.LoadLevel();

		if (m_currentState == GameState::RESTART) {
			m_currentState = m_previousState;
			m_nextState = m_previousState;
		}

		// Game loop
		while (m_currentState == m_nextState) {
			UpdateGameLoop();

			while (SDL_PollEvent(&event)) {
				switch (event.type) {
				case SDL_QUIT:

					break;
				}
			}
		}

		if (m_nextState != GameState::RESTART) {
			//TODO: Unload some assets
		}
		else {
			//TODO: Unload all assets
		}

		m_previousState = m_currentState;
		m_currentState = m_nextState;
	}
}

void GameStateManager::SetGameState(GameState state) { m_currentState = state; }

void GameStateManager::UpdateGameLoop() {
	// TODO: Clean up
	FrameRateManager& frameRateMngr = FrameRateManager::GetInstance();
	ResourceManager& resourceMngr = ResourceManager::GetInstance();
	RenderManager& renderMngr = RenderManager::GetInstance();
	GameObjectManager& gameObjectMngr = GameObjectManager::GetInstance();
	InputManager& inputMngr = InputManager::GetInstance();
	PhysicsManager& physicsMngr = PhysicsManager::GetInstance();
	DebugManager& debugMngr = DebugManager::GetInstance();
	AudioManager& audio = AudioManager::GetInstance();

	frameRateMngr.FrameStart();							// Record start of frame
	renderMngr.FrameStart();							// Clear depth and color
	float dt = frameRateMngr.GetFrameTime();			// Grab delta frame time

	inputMngr.Update();									// Update input keys
	gameObjectMngr.Update(dt);							// Update game logic
	gameObjectMngr.UpdateStatus();						// Update status of game objects
	physicsMngr.Integrate(dt);							// Move physics bodies
	physicsMngr.ResolveCollisions();					// Resolve collisions on physics bodies
	gameObjectMngr.LateUpdate(dt);						// Update game logic that occurs after physics
	audio.Update(dt);									//Update audio state				
	gameObjectMngr.RenderGameObjects();					// Render all game objects
	debugMngr.RenderDebugCommands(*gameObjectMngr.GetActiveCamera());

	renderMngr.FrameEnd();								// Swap window buffer
	frameRateMngr.FrameEnd();							// Lock FPS 
}