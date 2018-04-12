#include <Stdafx.h>


SplashScreen::SplashScreen():Component(ComponentType::C_SplashScreen), m_levelToLoad(0)
{
	TETRA_EVENTS.Subscribe(EVENT_OnScriptedAnimationComplete, this);
	TETRA_EVENTS.Subscribe(EVENT_INPUT_SCREENBYPASS, this);
}

SplashScreen::~SplashScreen(){}


void SplashScreen::Update(float dt)
{

}

void SplashScreen::Serialize(const json & j)
{
	m_levelToLoad = ParseInt(j, "levelToLoad");
}

void SplashScreen::HandleEvent(Event * pEvent)
{
	switch (pEvent->Type()) {
		case EVENT_OnScriptedAnimationComplete: {
			TETRA_LEVELS.ChangeLevel(m_levelToLoad);
			break;
		}
		case EVENT_INPUT_SCREENBYPASS: {
			InputButtonData* pData = pEvent->Data<InputButtonData>();
			if (pData->m_isReleased) {
				Audio* pAudio = pGO->GetComponent<Audio>(C_Audio);
				if (pAudio)
					pAudio->Play();
				TETRA_LEVELS.ChangeLevel(m_levelToLoad);
			}
			break;
		}
	}
}

void SplashScreen::LateInitialize()
{

}

