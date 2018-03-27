#pragma once
#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

class LevelManager: public Subscriber
{
private:
	int maxLevel;
	int currentLevel;
	int firstLevel;
	int mainMenuLevel;
	json levelConfig;
	json staticObjects;
	bool m_isRandomlyGenerated;
	bool m_wasRandomlyGenerated;
	int m_levelsCompleted;
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
	void LoadLevelEditor(int i);
	void NextLevel();
	void RestartGame();
	int GetLevelsCompleted() { return m_levelsCompleted; }
	void HandleEvent(Event* pEvent);
	bool IsLastLevel();
	int GetCurrentLevel();
	void LoadStaticGameObjects();
	void ActivateRandomGeneration(bool isActive) {	m_isRandomlyGenerated = isActive; }
};


#endif