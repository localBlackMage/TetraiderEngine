#include <Stdafx.h>

LetterBox::LetterBox() :
	Component(C_LetterBox)
{
}

void LetterBox::LateInitialize()
{
	if (!m_pTransform) {
		assert(pGO && "No Game Object found. LetterBox component failed to operate.");
		m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
		assert(m_pTransform && "No Transform component found. LetterBox component failed to operate.");

		switch (m_anchor) {
			case A_LEFT: {
				m_pTransform->SetScale(100.f, 2000.f, 0);
				m_pTransform->SetPosition(Vector3D(-600.f, 0, 0));
				break;
			}
			case A_RIGHT: {
				m_pTransform->SetScale(100.f, 2000.f, 0);
				m_pTransform->SetPosition(Vector3D(-600.f, 0, 0));
				break;
			}
			case A_TOP: {
				m_pTransform->SetScale(2000.f, 100.f, 0);
				m_pTransform->SetPosition(Vector3D(0, 400.f, 0));
				break;
			}
			case A_BOTTOM: {
				m_pTransform->SetScale(2000.f, 100.f, 0);
				m_pTransform->SetPosition(Vector3D(0, -400.f, 0));
				break;
			}
		}
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
}

void LetterBox::HandleEvent(Event * pEvent)
{
}