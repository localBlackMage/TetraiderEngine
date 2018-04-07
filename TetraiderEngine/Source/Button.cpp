//#include "Button.h"
//#include "TetraiderAPI.h"
//#include "Sprite.h"

#include <Stdafx.h>

Button::Button() :Component(ComponentType::C_Button) 
{
	m_isQuit = false;
	m_isRestart = false;
}
Button::~Button() {}

void Button::Update(float dt)
{
	m_pSprite->SetVOffset(0);
}

void Button::Serialize(const json & j)
{
	m_levelNumber = ParseInt(j, "level");
	m_isQuit = ParseBool(j, "quit");
	m_isRestart = ParseBool(j,"restart");
	m_isLoadCanvas = ParseBool(j, "isLoadCanvas");
	m_isCloseShop = ParseBool(j, "isCloseShop");
	if (m_isLoadCanvas) {
		m_canvasToActivate = (CanvasType)ParseInt(j, "canvasToActivate");
		m_canvasToDeActivate = (CanvasType)ParseInt(j, "canvasToDeActivate");
	}
	m_isRandomGenerated = ParseBool(j, "isRandomGenerated");
	m_isLoadLevelEditor = ParseBool(j, "isLoadLevelEditor");
}

void Button::LateInitialize()
{
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

void Button::HandleEvent(Event* pEvent)
{
	if (pEvent->Type() == EVENT_OnCollide)
	{
		OnCollideData* pData = pEvent->Data<OnCollideData>();
		if (pData->pGO->m_tag == T_Cursor) {
			m_pSprite->SetVOffset(0.3333f);
			if (TETRA_INPUT.IsMouseButtonPressed(MOUSEBTN::MOUSE_BTN_LEFT)) {
				m_pSprite->SetVOffset(0.6666f);
			}
			else if (TETRA_INPUT.IsMouseButtonReleased(MOUSEBTN::MOUSE_BTN_LEFT)) {
				if (m_isQuit) {
					TETRA_EVENTS.BroadcastEvent(&Event(EVENT_WINDOW_CLOSED));
				}
				else if (m_isRestart) {
					TETRA_LEVELS.ActivateRandomGeneration(false);
					TETRA_PLAYERSTATS.ClearStats();
					TETRA_LEVELS.ChangeLevel(m_levelNumber);
				}
				else if (m_isLoadLevelEditor) {
					TETRA_LEVELS.LoadLevelEditor(m_levelNumber);
				}
				else if(m_isLoadCanvas)
				{
					TETRA_UI.ActivateCanvas(m_canvasToActivate);
					TETRA_UI.DeactivateCanvas(m_canvasToDeActivate);
				}
				else if (m_isCloseShop) {
					TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EventType::EVENT_ShopClosed));
				}
				else {
					TETRA_LEVELS.ActivateRandomGeneration(m_isRandomGenerated);
					TETRA_LEVELS.ChangeLevel(m_levelNumber);
				}
			}
		}
	}
}

