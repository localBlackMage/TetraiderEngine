#include <Stdafx.h>

#define POWER_UP_SETTINGS "POWER_UP_SETTINGS"
#define POWER_UPS "POWER_UPS"

PlayerStatsManager::PlayerStatsManager(): m_isNewGame(true) {
	TETRA_EVENTS.Subscribe(EVENT_GoldenFeatherCollected, this);
	TETRA_EVENTS.Subscribe(EVENT_ExitLevel, this);
	TETRA_EVENTS.Subscribe(EVENT_LevelInComplete, this);
}
PlayerStatsManager::~PlayerStatsManager() {}

void PlayerStatsManager::HandleEvent(Event * p_event) {
	switch (p_event->Type()) {
		case EVENT_GoldenFeatherCollected: {
			CollectibleData* pData = p_event->Data<CollectibleData>();
			if (pData) {
				m_playerStats.m_goldenFeathers += pData->m_value;
			}
			TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_UIGoldenFeatherUpdate, &CollectibleData(m_playerStats.m_goldenFeathers)));
			break;
		}
		case EVENT_ExitLevel: {
			SaveStats();
			break;
		}
		case EVENT_LevelInComplete: {
			ClearStats();
			break;
		}
	}
}

void PlayerStatsManager::ClearStats() {
	for (int i = 0; i < 3; ++i) {
		m_powerUps[i].clear();
	}

	for (int i = 0; i < (int)PowerUpType::NUM; ++i) {
		m_activePowerUps[i] = nullptr;
	}

	InitializePowerUps(m_powerUpSettings);
	m_isNewGame = true;
}

void PlayerStatsManager::SaveStats() {
	const GameObject* pPlayer = TETRA_GAME_OBJECTS.GetPlayer();
	if (pPlayer) {
		const Health* pPlayerHealth = pPlayer->GetComponent<Health>(C_Health);
		m_playerStats.m_health = pPlayerHealth->GetHealth();
		m_playerStats.m_maxHealth = pPlayerHealth->GetMaxHealth();
		const Weapon* pPlayerWeapon = pPlayer->GetComponent<Weapon>(C_Weapon);
		m_playerStats.m_ammo = pPlayerWeapon->GetAmmo(1);
	}

	m_isNewGame = false;
}

void PlayerStatsManager::LoadStats() {
	if (m_isNewGame) return;

	GameObject* pPlayer = TETRA_GAME_OBJECTS.FindObjectWithTag(T_Player);
	if (pPlayer) {
		Health* pPlayerHealth = pPlayer->GetComponent<Health>(C_Health);
		pPlayerHealth->SetHealth(m_playerStats.m_health, m_playerStats.m_maxHealth);
		Weapon* pPlayerWeapon = pPlayer->GetComponent<Weapon>(C_Weapon);
		pPlayerWeapon->SetAmmo(1, m_playerStats.m_ammo);
	}
	else {
		TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EventType::EVENT_OnPlayerHeal, &HealthChangeData(m_playerStats.m_health, m_playerStats.m_maxHealth, Vector3D(), 0, false)));
	}

	TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_UIAmmoUpdate, &CollectibleData(m_playerStats.m_ammo)));
	TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_UIGoldenFeatherUpdate, &CollectibleData(m_playerStats.m_goldenFeathers)));
}

void PlayerStatsManager::InitializePowerUps(json& json) {
	m_powerUpSettings = json;
	for (int i = 0; i < 3; ++i) {
		for (unsigned int j = 0; j < json[POWER_UP_SETTINGS][i][POWER_UPS].size(); ++j) {
			PowerUp p;
			p.m_category = (PowerUpCategory)(i);
			p.m_type = (PowerUpType)(ParseInt(json[POWER_UP_SETTINGS][i][POWER_UPS][j], "enumType"));
			p.m_maxLevel = ParseInt(json[POWER_UP_SETTINGS][i][POWER_UPS][j], "maxLevel");
			p.m_upgradeValue = ParseInt(json[POWER_UP_SETTINGS][i][POWER_UPS][j], "upgradeValue");
			p.m_description = ParseString(json[POWER_UP_SETTINGS][i][POWER_UPS][j], "description");
			p.m_texture = ParseString(json[POWER_UP_SETTINGS][i][POWER_UPS][j], "sprtieTexture");
			p.m_currentLevel = 0;
			p.m_index = j;
			m_powerUps[i].push_back(p);
		}
	}

	//m_activePowerUps[1] = &m_powerUps[0][0];
}

void PlayerStatsManager::EquipPowerUp(PowerUpCategory category, PowerUpType type, int index) {
	if (m_powerUps[(int)category][index].m_currentLevel < m_powerUps[(int)category][index].m_maxLevel) {
		m_powerUps[(int)category][index].m_currentLevel += 1;
	}

	m_activePowerUps[(int)type] = &m_powerUps[(int)category][index];
	if (type == PowerUpType::HealthUpgrade) {
		m_playerStats.m_maxHealth += m_activePowerUps[(int)type]->m_upgradeValue;
		m_playerStats.m_health = m_playerStats.m_maxHealth;
	}
}

int PlayerStatsManager::GetGoldenFeathers() {
	return m_playerStats.m_goldenFeathers;
}

int PlayerStatsManager::GetAmmoPickUpUpgrade() {
	if (m_activePowerUps[(int)(PowerUpType::IncreaseAmmoPickUp)])
		return m_activePowerUps[(int)(PowerUpType::IncreaseAmmoPickUp)]->m_currentLevel*m_activePowerUps[(int)(PowerUpType::IncreaseAmmoPickUp)]->m_upgradeValue;
	else return 0;
}

int PlayerStatsManager::GetHealthUpgrade() {
	if (m_activePowerUps[(int)(PowerUpType::HealthUpgrade)])
		return m_activePowerUps[(int)(PowerUpType::HealthUpgrade)]->m_currentLevel*m_activePowerUps[(int)(PowerUpType::HealthUpgrade)]->m_upgradeValue;
	else return 0;
}

bool PlayerStatsManager::IsTripleShotActive() {
	return m_activePowerUps[(int)(PowerUpType::TripleShot)];
}

const PowerUp& PlayerStatsManager::GetRandomDefensePowerUp() {
	return m_powerUps[(int)PowerUpCategory::Defense][0];
}

const PowerUp& PlayerStatsManager::GetRandomOffensePowerUp() {
	return m_powerUps[(int)PowerUpCategory::Offense][0];
}

const PowerUp& PlayerStatsManager::GetRandomUtilityPowerUp() {
	return m_powerUps[(int)PowerUpCategory::Utility][0];
}

#undef POWER_UP_SETTINGS
#undef POWER_UPS