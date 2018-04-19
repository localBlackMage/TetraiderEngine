/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef GAME_STATE_MANAGER_H
#define GAME_STATE_MANAGER_H

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
	bool m_isShopOpen;
	bool m_isViewingCredits;
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
	bool IsShopOpen() { return m_isShopOpen;  }
	void SetViewingCredits(bool isActive) { m_isViewingCredits = isActive; }
};

#endif