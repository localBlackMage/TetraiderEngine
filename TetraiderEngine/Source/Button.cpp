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
}

void Button::LateInitialize()
{
	if (!m_pSprite) {
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
	if (pEvent->Type() == EVENT_OnCollide && TETRA_INPUT.IsMouseButtonTriggered(MOUSEBTN::MOUSE_BTN_LEFT))
	{
		//std::cout << "Button Released\n";
		m_pSprite->SetVOffset(0.66666);

		TETRA_LEVELS.ChangeLevel(m_levelNumber);
		//TETRA_LEVELS.NextLevel();
	}
	else if (pEvent->Type() == EVENT_OnCollide)
	{
		//m_pSprite->SetUOffset(0);
		m_pSprite->SetVOffset(0.3333);
	}
}

