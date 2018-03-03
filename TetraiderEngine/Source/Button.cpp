#include "Button.h"
#include "TetraiderAPI.h"
#include "Sprite.h"



Button::Button() :Component(ComponentType::C_Button) 
{
	isQuit = false;
	isRestart = false;
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
	isRestart = ParseBool(j,"restart");
	isRandomGenerated = ParseBool(j, "isRandomGenerated");
	isLooadLevelEditor = ParseBool(j, "isLoadLevelEditor");
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
		if (pData->pGO->m_tag == T_UI) {
			m_pSprite->SetVOffset(0.3333f);
			if (TETRA_INPUT.IsMouseButtonPressed(MOUSEBTN::MOUSE_BTN_LEFT)) {
				m_pSprite->SetVOffset(0.6666f);
			}
			else if (TETRA_INPUT.IsMouseButtonReleased(MOUSEBTN::MOUSE_BTN_LEFT)) {
				if (isQuit) {
					TETRA_EVENTS.BroadcastEvent(&Event(WINDOW_CLOSED));
				}
				else if (isRestart) {
					TETRA_EVENTS.BroadcastEvent(&Event(RESTART_LEVEL));
				}
				else if (isLooadLevelEditor) {
					TETRA_LEVELS.LoadLevelEditor(m_levelNumber);
				}
				else {
					TETRA_LEVELS.ActivateRandomGeneration(isRandomGenerated);
					TETRA_LEVELS.ChangeLevel(m_levelNumber);
				}
			}
		}
	}
}

