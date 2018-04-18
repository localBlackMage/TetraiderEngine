/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

void LetterBox::_SetPosAndScale()
{
	// If we're not in full screen, there's no need for the letterboxing, so hide it away
	if (!TETRA_RENDERER.GetFullScreenStatus()) {
		m_pTransform->SetScale(0, 0, 0);
		m_pTransform->SetPosition(Vector3D(-2000.f, 0, 0));
	}
	else {
		int index = TETRA_GAME_CONFIG.GetCurrentResolutionIndex();
		float scale = m_scalePerResolution[index];
		float position = m_posPerResolution[index];

		switch (m_anchor) {
			case A_LEFT:
			case A_RIGHT: {
				m_pTransform->SetScale(scale, 2000.f, 0);
				m_pTransform->SetPosition(Vector3D(position, 0, 0));
				break;
			}
			case A_BOTTOM:
			case A_TOP: {
				m_pTransform->SetScale(2000.f, scale, 0);
				m_pTransform->SetPosition(Vector3D(0, position, 0));
				break;
			}
		}
	}

}

LetterBox::LetterBox() :
	Component(C_LetterBox)
{
	TETRA_EVENTS.Subscribe(EventType::EVENT_WINDOW_RESIZED, this);
	TETRA_EVENTS.Subscribe(EventType::EVENT_ENTER_FULLSCREEN, this);
	TETRA_EVENTS.Subscribe(EventType::EVENT_LEAVE_FULLSCREEN, this);
}

void LetterBox::LateInitialize()
{
	if (!m_pTransform) {
		assert(pGO && "No Game Object found. LetterBox component failed to operate.");
		m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
		assert(m_pTransform && "No Transform component found. LetterBox component failed to operate.");

		_SetPosAndScale();
	}
}

void LetterBox::Deactivate()
{
	pGO = nullptr;
}

void LetterBox::Serialize(const json & j)
{
	std::string anchor = ParseString(j, "Anchor");

	if (anchor == "Bottom")		m_anchor = A_BOTTOM;
	else if (anchor == "Top")	m_anchor = A_TOP;
	else if (anchor == "Left")	m_anchor = A_LEFT;
	else						m_anchor = A_RIGHT;

	for (int i = 0; i < MAX_RESOLUTIONS; ++i)
	{
		m_scalePerResolution[i] = j["Dimensions"][i];
		m_posPerResolution[i] = j["Positions"][i];
	}
}

void LetterBox::HandleEvent(Event * pEvent)
{
	switch (pEvent->Type()) {
		case EVENT_ENTER_FULLSCREEN:
		case EVENT_LEAVE_FULLSCREEN:
		case EVENT_WINDOW_RESIZED:
		{
			_SetPosAndScale();
			break;
		}
	}
}