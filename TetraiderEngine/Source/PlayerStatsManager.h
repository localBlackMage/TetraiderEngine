#pragma once
#ifndef PLAYER_STAT_MANAGER_H
#define PLAYER_STAT_MANAGER_H

enum class PowerUpCategory {
	Offense,
	Defense,
	Utility,

	NUM
};

enum class PowerUpType {
	HealthUpgrade,
	TripleShot,
	IncreaseAmmoPickUp,
	/*MovementSpeedUpgrade,
	IncreaseDamage,
	IncreaseAttackSpeed,*/

	NUM
};

struct PowerUp {
	PowerUpCategory m_category;
	PowerUpType m_type;
	int m_maxLevel;
	int m_currentLevel;
	int m_upgradeValue;
	int m_index;
	std::string m_description;
	std::string m_texture;
};

struct PlayerStats {
public:
	PlayerStats() : m_health(0), m_maxHealth(0), m_maxStamina(0), m_ammo(0), m_goldenFeathers(0) {}
	int m_health;
	int m_maxHealth;
	int m_maxStamina;
	int m_ammo;
	int m_goldenFeathers;
};

class PlayerStatsManager : public Subscriber
{
private:
	std::vector<PowerUp> m_powerUps[(int)PowerUpCategory::NUM];
	PowerUp* m_activePowerUps[(int)PowerUpType::NUM];
	PlayerStats m_playerStats;
	json m_powerUpSettings;
	bool m_isNewGame;
public:
	PlayerStatsManager();
	~PlayerStatsManager();
	PlayerStatsManager(const PlayerStatsManager &) = delete;
	void operator=(const PlayerStatsManager &) = delete;
	virtual void HandleEvent(Event* p_event);
	int GetGoldenFeathers();
	int GetAmmoPickUpUpgrade();
	int GetHealthUpgrade();
	bool IsTripleShotActive();
	void ClearStats();
	void SaveStats();
	void LoadStats();
	void InitializePowerUps(json& j);
	void EquipPowerUp(PowerUpCategory category, PowerUpType type, int index);
	const PowerUp& GetRandomDefensePowerUp();
	const PowerUp& GetRandomOffensePowerUp();
	const PowerUp& GetRandomUtilityPowerUp();
};

#endif