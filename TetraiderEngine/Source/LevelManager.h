#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include "JsonReader.h"
#include "Event.h"

using json = nlohmann::json;

class LevelManager
{
private:
	LevelManager();
	~LevelManager();

	void LoadLevel(json j);
	int maxLevel;
	int currentLevel;
	int firstLevel;
	json levelConfig;
public:
	LevelManager(const LevelManager &) = delete;
	void operator=(const LevelManager &) = delete;

	static LevelManager& GetInstance(unsigned int maxFrameRate = 60)
	{
		static LevelManager instance;
		return instance;
	}

	void Initialize(json j);
	void LoadLevel();
	void UnLoadLevel();
	void UnLoadLevelForRestart();
	void ChangeLevel(int i);
	void NextLevel();
	void RestartGame();
	bool IsLastLevel();
};

class OnLevelInitialized : public Event {
public:
	OnLevelInitialized() : Event(EVENT_OnLevelInitialized) {}
	~OnLevelInitialized() {}
};

#endif