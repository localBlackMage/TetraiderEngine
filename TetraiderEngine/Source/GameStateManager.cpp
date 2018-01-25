#include "GameStateManager.h"
#include "LevelManager.h"
#include "TetraiderAPI.h"

GameStateManager::GameStateManager() : 
	m_previousState(GameState::CURRENT_LEVEL), 
	m_currentState(GameState::CURRENT_LEVEL), 
	m_nextState(GameState::CURRENT_LEVEL) 
{}

GameStateManager::~GameStateManager() {}

void GameStateManager::Update() {
	LevelManager& levelMngr = LevelManager::GetInstance();
	
	while (m_currentState != GameState::QUIT) {
		levelMngr.LoadLevel();

		if (m_currentState == GameState::RESTART) {
			m_currentState = m_previousState;
			m_nextState = m_previousState;
		}

		// Game loop
		while (m_currentState == m_nextState) {
			Tetraider::FrameStart();
			Tetraider::Update(Tetraider::GetFrameTime());
			Tetraider::FrameEnd();
		}

		if (m_nextState != GameState::RESTART) {
			//TODO: Unload some assets
		}
		else {
			Tetraider::UnloadResources();
		}

		m_previousState = m_currentState;
		m_currentState = m_nextState;
	}
}

void GameStateManager::SetGameState(GameState state) { m_currentState = state; }
