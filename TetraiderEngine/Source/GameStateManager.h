#pragma once
#ifndef GAME_STATE_MANAGER_H
#define GAME_STATE_MANAGER_H

//#include "Subscriber.h"
//
//class LevelManager;
//class LevelEditor;

enum class GameState {
	QUIT,
	RESTART,
	CURRENT_LEVEL,
	NEXT_LEVEL,

	NONE
};

class GameStateManager : public Subscriber
{
private:
	GameState m_currentState;
	GameState m_nextState;
	bool m_debugPause;
	bool m_isGamePaused;
	bool m_isLevelOver;
	bool m_isLevelEditorMode;

	void SetGameState(GameState);
public:
	GameStateManager();
	~GameStateManager();
	friend class LevelManager;
	GameStateManager(const GameStateManager &) = delete;
	void operator=(const GameStateManager &) = delete;
	void Update();
	virtual void HandleEvent(Event* p_event);
	bool IsDebugPause() { return m_debugPause; }
	void PauseGame(bool active) {  m_isGamePaused = active; }
	bool IsGamePaused() { return m_isGamePaused; }
	bool IsEditorMode() { return m_isLevelEditorMode; }
};

#endif