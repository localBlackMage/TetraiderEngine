#pragma once
#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include "JsonReader.h"
#include "Event.h"

using json = nlohmann::json;

class LevelManager
{
private:
	void LoadLevel(json j);
	int maxLevel;
	int currentLevel;
	int firstLevel;
	json levelConfig;
public:
	LevelManager();
	~LevelManager();
	LevelManager(const LevelManager &) = delete;
	void operator=(const LevelManager &) = delete;

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
	OnLevelInitialized() : Event(EventType::EVENT_OnLevelInitialized) {}
	~OnLevelInitialized() {}
};

#endif