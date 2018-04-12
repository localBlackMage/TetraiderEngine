#pragma once
#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

enum BossAndShop : short {
	NEITHER = 0,
	BOSS_ONLY,
	SHOP_ONLY,
	BOSS_SHOP
};

struct LevelConfig {
	unsigned short cols;
	unsigned short rows;
	unsigned short difficulty;
	unsigned short eggsToCollect;
	BossAndShop bossAndShop;
};

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
	short m_maxLevel;							// Maximum level in a randomly-generated playthrough
	std::vector< LevelConfig > m_levelConfigs;	// Contains configuration info. for each level

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
	void ResetGame();
	void IncrementLevelCompleted();
	bool IsFinalLevel();
	void LoadMainMenu();
	int GetLevelsCompleted() { return m_levelsCompleted; }
	int EggsToCollect() { return m_levelConfigs[m_levelsCompleted].eggsToCollect; }
	void HandleEvent(Event* pEvent);
	bool IsLastLevel();
	int GetCurrentLevel();
	void LoadStaticGameObjects();
	void ActivateRandomGeneration(bool isActive) {	m_isRandomlyGenerated = isActive; }
};

#endif