#ifndef GAME_STATE_MANAGER_H
#define GAME_STATE_MANAGER_H

enum class GameState {
	QUIT,
	RESTART,
	CURRENT_LEVEL,
	NEXT_LEVEL,

	NONE
};

class GameStateManager
{
private:
	GameStateManager();
	~GameStateManager();

	GameState m_previousState;
	GameState m_currentState;
	GameState m_nextState;
	//void UpdateGameLoop();
public:
	GameStateManager(const GameStateManager &) = delete;
	void operator=(const GameStateManager &) = delete;

	static GameStateManager& GetInstance()
	{
		static GameStateManager instance;
		return instance;
	}

	void SetGameState(GameState);
	void Update();
};

#endif