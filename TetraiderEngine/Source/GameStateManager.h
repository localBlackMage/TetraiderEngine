#pragma once
#ifndef GAME_STATE_MANAGER_H
#define GAME_STATE_MANAGER_H

#include "Subscriber.h"

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
	GameState m_previousState;
	GameState m_currentState;
	GameState m_nextState;
	bool m_debugPause;
	bool m_isGamePaused;
public:
	GameStateManager();
	~GameStateManager();
	GameStateManager(const GameStateManager &) = delete;
	void operator=(const GameStateManager &) = delete;

	void SetGameState(GameState);
	void Update();
	virtual void HandleEvent(Event* p_event);
	bool IsDebugPause() { return m_debugPause; }
	void PauseGame(bool active) {  m_isGamePaused = active; }
	bool IsGamePaused() { return m_isGamePaused; }
};

#endif