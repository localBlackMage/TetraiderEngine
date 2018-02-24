#pragma once
#ifndef GAME_STATE_MANAGER_H
#define GAME_STATE_MANAGER_H

#include "Subscriber.h"

class LevelManager;

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

	void SetGameState(GameState);
public:
	GameStateManager();
	~GameStateManager();
	friend LevelManager;
	GameStateManager(const GameStateManager &) = delete;
	void operator=(const GameStateManager &) = delete;
	void Update();
	virtual void HandleEvent(Event* p_event);
	bool IsDebugPause() { return m_debugPause; }
	void PauseGame(bool active) {  m_isGamePaused = active; }
	bool IsGamePaused() { return m_isGamePaused; }
};

#endif