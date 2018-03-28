#pragma once
#ifndef PLAYER_STAT_MANAGER_H
#define PLAYER_STAT_MANAGER_H


class PlayerStatsManager : public Subscriber
{
private:
	std::vector<PowerUp> m_powerUps[(int)PowerUpCategory::NUM];
	PowerUp* m_activePowerUps[(int)PowerUpType::NUM];
	PlayerStats m_playerStats;
	json m_powerUpSettings;
	int numberOfNormalPowerUps;
	int numberOfSpecialPowerUps;
	bool m_isNewGame;
public:
	PlayerStatsManager();
	~PlayerStatsManager();
	PlayerStatsManager(const PlayerStatsManager &) = delete;
	void operator=(const PlayerStatsManager &) = delete;
	virtual void HandleEvent(Event* p_event);
	int GetGoldenFeathers();
	bool IsPowerUpActive(PowerUpType p);
	bool IsPowerUpActive(PowerUpType p, int &upgradeValue);
	bool IsPowerUpActive(PowerUpType p, float &upgradeValue);
	void ClearStats();
	void SaveStats();
	void LoadStats();
	void InitializePowerUps(json& j);
	bool EquipPowerUp(PowerUpCategory category, PowerUpType type, int index);
	bool IsEnoughGoldenFeather(PowerUpCategory category, PowerUpType type, int index);
	const PowerUp& GetRandomNormalPowerUp();
	const PowerUp& GetSpecialPowerUp(int i);
};

#endif