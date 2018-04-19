/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef POWER_UP_DATA_H
#define POWER_UP_DATA_H

enum class PowerUpCategory {
	Special,
	Normal,

	NUM
};

enum class PowerUpType {
	IncreaseMeleeDamage,
	IncreaseRangeDamage,
	TripleShot,

	HealthUpgrade,
	IncreaseHealthFromPickUp,
	IncreaseMeleeRange,
	IncreaseAmmoPickUp,
	IncreaseStamina,
	IncreaseAgility,

	NUM
};

struct PowerUp {
	PowerUpCategory m_category;
	PowerUpType m_type;
	int m_maxLevel;
	int m_currentLevel;
	int m_cost;
	float m_upgradeValue;
	int m_index;
	bool m_isAvailable;
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

#endif