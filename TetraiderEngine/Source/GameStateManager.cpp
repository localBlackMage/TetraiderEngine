#include "GameStateManager.h"

GameStateManager::GameStateManager(): m_previousState(GameState::CURRENT_LEVEL), m_currentState(GameState::CURRENT_LEVEL), m_nextState(GameState::CURRENT_LEVEL) {}

GameStateManager::~GameStateManager() {}

void GameStateManager::Update() {
	while (m_currentState != GameState::QUIT) {
		// Load level

		if (m_currentState == GameState::RESTART) {
			m_currentState = m_previousState;
			m_nextState = m_previousState;
		}

		while (m_currentState == m_nextState) {
			// Update game loop
			UpdateGameLoop();
		}

		if (m_nextState != GameState::RESTART) {
			// Unload some assets
		}
		else {
			// Unload all assets
		}

		m_previousState = m_currentState;
		m_currentState = m_nextState;
	}
}
