#include "GameStateManager.h"
#include "TetraiderAPI.h"

GameStateManager::GameStateManager() :
	m_previousState(GameState::CURRENT_LEVEL),
	m_currentState(GameState::CURRENT_LEVEL),
	m_nextState(GameState::CURRENT_LEVEL),
	m_debugPause(false)
{}

GameStateManager::~GameStateManager() {}

void GameStateManager::Update() {
	while (m_currentState != GameState::QUIT) {
		//TETRA_LEVELS.LoadLevel();
		TETRA_LEVEL_GEN.GenerateFloorPlan();
		TETRA_LEVEL_GEN.PrintFloorPlan();
		TETRA_LEVEL_GEN.GenerateLevelFromFloorPlan();

		if (m_currentState == GameState::RESTART) {
			m_currentState = m_previousState;
			m_nextState = m_previousState;
		}

		// Game loop
		while (m_currentState == m_nextState) {
			// NOTE: This condition should be before frame start
			if (TETRA_INPUT.IsKeyTriggered(SDL_SCANCODE_F2))
				m_debugPause = !m_debugPause; 

			Tetraider::FrameStart();

			if(!m_debugPause)
				Tetraider::Update(Tetraider::GetFrameTime());	// Game loop
			else
				Tetraider::DebugMode();							// Debug mode

			Tetraider::FrameEnd();
		}

		if (m_nextState == GameState::RESTART) {
			TETRA_LEVELS.UnLoadLevelForRestart();
		}
		else if(m_nextState == GameState::NEXT_LEVEL) {
			//TODO: Unload some assets
		}

		m_previousState = m_currentState;
		m_currentState = m_nextState;
	}

	Tetraider::UnloadResources();	// Unloads all resources
}

void GameStateManager::HandleEvent(Event * p_event)
{
	if (p_event->Type() == EventType::WINDOW_CLOSED)
		m_nextState = GameState::QUIT;
	else if (p_event->Type() == EventType::RESTART_LEVEL)
		m_nextState = GameState::RESTART;
}

void GameStateManager::SetGameState(GameState state) { m_currentState = state; }
