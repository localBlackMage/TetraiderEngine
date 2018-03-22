//#include "GameStateManager.h"
//#include "TetraiderAPI.h"

#include <Stdafx.h>

GameStateManager::GameStateManager() :
	m_currentState(GameState::CURRENT_LEVEL),
	m_nextState(GameState::CURRENT_LEVEL),
	m_debugPause(false),
	m_isLevelEditorMode(false),
	m_isShopOpen(false)
{
	TETRA_EVENTS.Subscribe(EVENT_INPUT_PAUSEGAME, this);
	TETRA_EVENTS.Subscribe(EVENT_ExitLevel, this);
	TETRA_EVENTS.Subscribe(EVENT_LevelInComplete, this);
	TETRA_EVENTS.Subscribe(EVENT_ShopOpened, this);
	TETRA_EVENTS.Subscribe(EVENT_ShopClosed, this);
}

GameStateManager::~GameStateManager() {}

void GameStateManager::Update() {
	while (m_currentState != GameState::QUIT) {
		TETRA_LEVELS.LoadLevel();

		m_currentState = GameState::CURRENT_LEVEL;
		m_nextState = GameState::CURRENT_LEVEL;

		// Game loop
		while (m_currentState == m_nextState) {
			// NOTE: This condition should be before frame start
			if (TETRA_INPUT.IsKeyTriggered(SDL_SCANCODE_F2))
				m_debugPause = !m_debugPause; 

			Tetraider::FrameStart();

			if (m_isLevelEditorMode) Tetraider::LevelEditorMode(Tetraider::GetFrameTime()); // Level editor mode
			else if(!m_debugPause) Tetraider::Update(Tetraider::GetFrameTime());			// Game loop
			else Tetraider::DebugMode();													// Debug mode

			Tetraider::FrameEnd();
		}

		if (m_nextState == GameState::RESTART) {
			TETRA_LEVELS.UnLoadLevelForRestart();
		}
		else if(m_nextState == GameState::NEXT_LEVEL) {
			TETRA_LEVELS.UnLoadLevel();
		}

		m_currentState = m_nextState;
	}

	Tetraider::UnloadResources();	// Unloads all resources
}

void GameStateManager::HandleEvent(Event * p_event) {
	switch (p_event->Type()) {
		case WINDOW_CLOSED: {
			SetGameState(GameState::QUIT);
			break;
		}
		case RESTART_LEVEL: {
			SetGameState(GameState::RESTART);
			break;
		}
		case EVENT_INPUT_PAUSEGAME: {
			if (m_isLevelOver || m_isShopOpen) return;
			InputButtonData* pData = p_event->Data<InputButtonData>();
			if(pData->m_isTrigger) PauseGame(!m_isGamePaused);
			break;
		}
		case EVENT_LevelInComplete: {
			m_isLevelOver = true;
			PauseGame(true);
			break;
		}
		case EVENT_ExitLevel: {
			m_isLevelOver = true;
			break;
		}
		case EVENT_ShopOpened: {
			m_isShopOpen = true;
			break;
		}
		case EVENT_ShopClosed: {
			m_isShopOpen = false;
			break;
		}
	}
}

void GameStateManager::SetGameState(GameState state) { 
	m_nextState = state; 
	PauseGame(false);
	m_isLevelOver = false;
}
