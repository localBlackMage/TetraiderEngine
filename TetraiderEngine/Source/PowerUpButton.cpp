#include <Stdafx.h>

PowerUpButton::PowerUpButton() :Component(ComponentType::C_PowerUpButton) {}
PowerUpButton::~PowerUpButton() {}

void PowerUpButton::Update(float dt) {
	m_pSprite->SetVOffset(0);
}

void PowerUpButton::Serialize(const json & j) {
	std::string category = ParseString(j, "category");
	
	if (category == "Normal") {
		m_powerUp = TETRA_PLAYERSTATS.GetRandomNormalPowerUp();
	}
	else if (category == "Special") {
		//m_powerUp = TETRA_PLAYERSTATS.GetRandomDefensePowerUp();
	}

	m_levelNumber = ParseInt(j, "level");
	m_powerUpIconPrefab = ParseString(j, "iconPrefab");
}

void PowerUpButton::LateInitialize() {
	if (!m_pSprite) 
	{
		if (pGO)
			m_pSprite = pGO->GetComponent<Sprite>(ComponentType::C_Sprite);
		else {
			printf("No Game Object found. Button component failed to operate.\n");
			return;
		}

		if (!m_pSprite) {
			printf("No Sprite component found. Button component failed to operate.\n");
			assert(m_pSprite);
			return;
		}
	}
}

void PowerUpButton::HandleEvent(Event * pEvent)
{
	if (pEvent->Type() == EVENT_OnLevelInitialized) {
		GameObject* pGameObject = TETRA_GAME_OBJECTS.FindObjectWithTag(T_PowerUpText);
		m_pPowerUpText = pGameObject->GetComponent<Text>(C_Text);
		GameObject* pIconGO = TETRA_GAME_OBJECTS.CreateGameObject(m_powerUpIconPrefab, true, pGO->GetComponent<Transform>(C_Transform)->GetPosition());
		Sprite* pSprite = pIconGO->GetComponent<Sprite>(C_Sprite);
		pSprite->SetSprite(m_powerUp.m_texture);
	}
	else if (pEvent->Type() == EVENT_OnCollide)
	{
		OnCollideData* pData = pEvent->Data<OnCollideData>();
		if (pData->pGO->m_tag == T_Cursor) {
			m_pSprite->SetVOffset(0.3333f);
			m_pPowerUpText->SetText(m_powerUp.m_description);
			if (TETRA_INPUT.IsMouseButtonPressed(MOUSEBTN::MOUSE_BTN_LEFT)) {
				m_pSprite->SetVOffset(0.6666f);
			}
			else if (TETRA_INPUT.IsMouseButtonReleased(MOUSEBTN::MOUSE_BTN_LEFT)) {
				TETRA_PLAYERSTATS.EquipPowerUp(m_powerUp.m_category, m_powerUp.m_type, m_powerUp.m_index);
				TETRA_LEVELS.ActivateRandomGeneration(true);
				TETRA_LEVELS.ChangeLevel(m_levelNumber);
			}
		}
	}
}

