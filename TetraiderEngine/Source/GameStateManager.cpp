#include "GameStateManager.h"
#include "FrameRateManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "GameObjectManager.h"
#include "InputManager.h"
#include "LevelManager.h"


#include "GameObject.h"

GameStateManager::GameStateManager(): m_previousState(GameState::CURRENT_LEVEL), m_currentState(GameState::CURRENT_LEVEL), m_nextState(GameState::CURRENT_LEVEL) {}

GameStateManager::~GameStateManager() {}

void GameStateManager::Update() {
	// TODO: Clean up
	FrameRateManager& frameRateMngr = FrameRateManager::GetInstance();
	ResourceManager& resourceMngr = ResourceManager::GetInstance();
	RenderManager& renderMngr = RenderManager::GetInstance();
	GameObjectManager& gameObjectMngr = GameObjectManager::GetInstance();
	InputManager& inputMngr = InputManager::GetInstance();
	LevelManager& levelMngr = LevelManager::GetInstance();

	Mesh * pMesh = resourceMngr.GetMesh("quad");
	GameObject * pGO = gameObjectMngr.CreateGameObject("P_Test");
	float dt = 1 / 60.0f;

	SDL_Event event;

	while (m_currentState != GameState::QUIT) {
		levelMngr.LoadLevel();

		if (m_currentState == GameState::RESTART) {
			m_currentState = m_previousState;
			m_nextState = m_previousState;
		}
		//-------------------------------- GAME LOOP STARTS ----------------------------------------------------//
		while (m_currentState == m_nextState) {
			frameRateMngr.FrameStart();
			dt = frameRateMngr.GetFrameTime();
			// inputMngr.Update();					// Update input keys
			renderMngr.FrameStart();				// Clear depth Check if this needs to be done after game logic

			gameObjectMngr.Update(dt);				// Update game logic
			gameObjectMngr.UpdateStatus();			// Update status of game objects

			// TODO: Update to render all game objects
			renderMngr.RenderGameObject();
			//renderMngr.RenderSTB(nullptr, pMesh);   // Draw elements 

			renderMngr.FrameEnd();
			frameRateMngr.FrameEnd();

			//TODO: Move logic to update game loop
			// UpdateGameLoop();

			while (SDL_PollEvent(&event)) {
				switch (event.type) {
				case SDL_QUIT:
					break;
				}
			}
		}
		//-------------------------------- GAME LOOP ENDS ----------------------------------------------------//

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