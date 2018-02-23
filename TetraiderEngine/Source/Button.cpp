#include "Button.h"
#include "TetraiderAPI.h"
#include "Sprite.h"



Button::Button() :Component(ComponentType::C_Button) 
{
}
Button::~Button() {}

void Button::Update(float dt)
{
	m_pSprite->SetVOffset(0);
}

void Button::Serialize(const json & j)
{
	m_levelNumber = ParseInt(j, "level");
	isQuit = ParseBool(j, "quit");
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
		m_pSprite->SetVOffset(0.3333f);
		if (TETRA_INPUT.IsMouseButtonPressed(MOUSEBTN::MOUSE_BTN_LEFT)) {
			m_pSprite->SetVOffset(0.6666f);
		}
		else if (TETRA_INPUT.IsMouseButtonReleased(MOUSEBTN::MOUSE_BTN_LEFT)) {
			if (isQuit)
			{
				TETRA_GAME_STATE.SetGameState(GameState::QUIT);
			}
			else
				TETRA_LEVELS.ChangeLevel(m_levelNumber);
		}
	}
}

