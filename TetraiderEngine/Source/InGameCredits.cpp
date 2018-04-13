#include <Stdafx.h>

InGameCredits::InGameCredits(): Component(ComponentType::C_InGameCredits) {}
InGameCredits::~InGameCredits() {}

void InGameCredits::Update(float dt) {

}

void InGameCredits::Serialize(const json& j) {

}

void InGameCredits::LateInitialize() {
	TETRA_EVENTS.Subscribe(EVENT_OnCanvasActivated, this);
	TETRA_EVENTS.Subscribe(EVENT_INPUT_SCREENBYPASS, this);
	TETRA_EVENTS.Subscribe(EVENT_OnScriptedAnimationComplete, this);
}

void InGameCredits::HandleEvent(Event* pEvent) {
	if (!pGO->m_isActive) return; 

	switch (pEvent->Type())
	{
		case EVENT_OnCanvasActivated: {
			CanvasData* pData = pEvent->Data<CanvasData>();
			if (pData->m_canvasType == (int)CanvasType::CANVAS_CREDITS) {
				pGO->GetComponent<ScriptedAnimation>(C_ScriptedAnimation)->PlayAnimation();
				TETRA_GAME_STATE.SetViewingCredits(true);
			}
			break;
		}
		case EVENT_INPUT_SCREENBYPASS: {
			InputButtonData* pData = pEvent->Data<InputButtonData>();
			if (pData->m_isReleased) {
				Audio* pAudio = pGO->GetComponent<Audio>(C_Audio);
				if (pAudio)
					pAudio->Play();
				TETRA_UI.m_ignoreButtonsForOneFrame = true;
				pGO->GetComponent<ScriptedAnimation>(C_ScriptedAnimation)->StopPlaying();
				TETRA_UI.DeactivateCanvas(CanvasType::CANVAS_CREDITS);
				TETRA_UI.ActivateCanvas(CanvasType::CANVAS_PAUSE);
				TETRA_GAME_STATE.SetViewingCredits(false);
			}
			break;
		}
		case EVENT_OnScriptedAnimationComplete: {
			TETRA_UI.DeactivateCanvas(CanvasType::CANVAS_CREDITS);
			TETRA_UI.ActivateCanvas(CanvasType::CANVAS_PAUSE);
			TETRA_GAME_STATE.SetViewingCredits(false);
			break;
		}
	}
}