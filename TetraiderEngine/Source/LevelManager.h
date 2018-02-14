#pragma once
#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include "JsonReader.h"

using json = nlohmann::json;

class LevelManager
{
private:
	int maxLevel;
	int currentLevel;
	int firstLevel;
	json levelConfig;
	json staticObjects;
	bool m_isRandomlyGenerated;

	void _LoadLevel(const json& j);
public:
	LevelManager();
	~LevelManager();
	LevelManager(const LevelManager &) = delete;
	void operator=(const LevelManager &) = delete;

	void Initialize(const json& j);
	std::vector<GameObject*> LoadRoomFile(const json& j);
	void LoadLevel();
	void UnLoadLevel();
	void UnLoadLevelForRestart();
	void ChangeLevel(int i);
	void NextLevel();
	void RestartGame();
	bool IsLastLevel();

	void LoadStaticGameObjects();
};


#endif