#include <Stdafx.h>

PowerUpButton::PowerUpButton() :Component(ComponentType::C_PowerUpButton) {
	TETRA_EVENTS.Subscribe(EventType::EVENT_OnCanvasActivated, this);
	TETRA_EVENTS.Subscribe(EventType::EVENT_OnCanvasDeactivated, this);
}
PowerUpButton::~PowerUpButton() {}

void PowerUpButton::Update(float dt) {
	if (m_isSpecial && TETRA_PLAYERSTATS.IsPowerUpActive(m_powerUp.m_type)) {
			m_pPrice->SetActive(false);
			m_pSprite->SetVOffset(0.6666f);
	}
	else
		m_pSprite->SetVOffset(0);
}

void PowerUpButton::LateUpdate(float dt) {
	if (!m_isHover && m_pSprite->GetVOffset() > 0 && m_pSprite->GetVOffset() < 0.5f) {
		m_isHover = true;
		Audio* pAudio = pGO->GetComponent<Audio>(C_Audio);
		if (pAudio)
			pAudio->Play(0);
	}
	else if (m_isHover && m_pSprite->GetVOffset() < 0.1f) {
		m_isHover = false;
	}
}

void PowerUpButton::Serialize(const json & j) {
	std::string category = ParseString(j, "category");
	m_specialPowerIndex = ParseInt(j, "specialPowerIndex");
	if (category == "Normal") {
		m_powerUp = TETRA_PLAYERSTATS.GetRandomNormalPowerUp();
		m_isSpecial = false;
	}
	else if (category == "Special") {
		m_powerUp = TETRA_PLAYERSTATS.GetSpecialPowerUp(m_specialPowerIndex);
		m_isSpecial = true;
	}

	m_levelNumber = ParseInt(j, "level");
	m_powerUpIconPrefab = ParseString(j, "iconPrefab");
	m_powerUpPrice = ParseString(j, "powerUpPricePrefab");
	m_offsetForPrice = ParseVector3D(j, "priceOffset");
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
		GameObject* pIconGO = TETRA_GAME_OBJECTS.CreateGameObject(m_powerUpIconPrefab, true, pGO->GetComponent<Transform>(C_Transform)->GetLocalPosition());
		Sprite* pSprite = pIconGO->GetComponent<Sprite>(C_Sprite);
		pSprite->SetSprite(m_powerUp.m_texture);
		if (m_isSpecial) {
			TETRA_UI.AddGameObjectToCanvas(CanvasType::CANVAS_SHOP, pIconGO);
			pIconGO->SetActive(false);
			m_pPrice = TETRA_GAME_OBJECTS.CreateGameObject(m_powerUpPrice, true, pGO->GetComponent<Transform>(C_Transform)->GetLocalPosition() + m_offsetForPrice);
			Text* pText = m_pPrice->GetComponent<Text>(C_Text);
			pText->SetText(std::to_string(m_powerUp.m_cost));
			TETRA_UI.AddGameObjectToCanvas(CanvasType::CANVAS_SHOP, m_pPrice);
			m_pPrice->SetActive(false);
		}
		else {
			TETRA_UI.AddGameObjectToCanvas(CanvasType::CANVAS_POWERUPSCREEN, pIconGO);
		}
	}
	else if (pEvent->Type() == EVENT_OnCollide)
	{
		if (m_isSpecial && TETRA_PLAYERSTATS.IsPowerUpActive(m_powerUp.m_type)) return;

		OnCollideData* pData = pEvent->Data<OnCollideData>();
		if (pData->pGO->m_tag == T_Cursor) {
			m_pSprite->SetVOffset(0.3333f);
			m_pPowerUpText->SetText(m_powerUp.m_description);
			if (m_isSpecial) {
				if(!TETRA_PLAYERSTATS.IsEnoughGoldenFeather(m_powerUp.m_category, m_powerUp.m_type, m_powerUp.m_index))
					TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EventType::EVENT_NotEnoughGoldenFeathers));
				else
					TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EventType::EVENT_EnoughGoldenFeathers));
			}

			if (TETRA_INPUT.IsMouseButtonPressed(MOUSEBTN::MOUSE_BTN_LEFT)) {
				m_pSprite->SetVOffset(0.6666f);
			}
			else if (TETRA_INPUT.IsMouseButtonReleased(MOUSEBTN::MOUSE_BTN_LEFT)) {
				if (!m_isSpecial) {
					TETRA_PLAYERSTATS.EquipPowerUp(m_powerUp.m_category, m_powerUp.m_type, m_powerUp.m_index);
					Audio* pAudio = pGO->GetComponent<Audio>(C_Audio);
					if (pAudio)
						pAudio->Play(1);
					TETRA_UI.DeactivateCanvas(CanvasType::CANVAS_POWERUPSCREEN);
				}
				else {
					TETRA_PLAYERSTATS.EquipPowerUp(m_powerUp.m_category, m_powerUp.m_type, m_powerUp.m_index);
					TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_PowerUpPurchased));
				}
			}
		}
	}
}

