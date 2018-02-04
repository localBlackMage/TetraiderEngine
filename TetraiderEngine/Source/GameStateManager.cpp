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

	//start back ground music
	//TETRA_AUDIO.PlaySong("../TetraiderEngine/Assets/SFX/bgm.mp3");

	while (m_currentState != GameState::QUIT) {
		TETRA_LEVELS.LoadLevel();

		if (m_currentState == GameState::RESTART) {
			m_currentState = m_previousState;
			m_nextState = m_previousState;
		}

		// Game loop
		while (m_currentState == m_nextState) {
			if (TETRA_INPUT.IsKeyTriggered(SDL_SCANCODE_F2)) { m_debugPause = !m_debugPause; } // Triggers debug mode

			Tetraider::FrameStart();

			if(!m_debugPause)
				Tetraider::Update(Tetraider::GetFrameTime());	// Game loop
			else
				Tetraider::DebugMode();							// Debug mode

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

void GameStateManager::HandleEvent(Event * p_event)
{
	if (p_event->Type() == EventType::WINDOW_CLOSED)
		m_nextState = GameState::QUIT;
}

void GameStateManager::SetGameState(GameState state) { m_currentState = state; }
