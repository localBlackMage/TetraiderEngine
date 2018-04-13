#include "SelectableButton.h"
#include "Stdafx.h"

SelectableButton::SelectableButton():Component(ComponentType::C_SelectableButton)
{
}

SelectableButton::~SelectableButton()
{
}

void SelectableButton::Update(float dt)
{
}

void SelectableButton::Serialize(const json & j)
{
	m_isRightSelectable = ParseBool(j, "isRightSelectable");
	m_isLeftSelectable= ParseBool(j, "isLeftSelectable");
}

void SelectableButton::HandleEvent(Event * pEvent)
{
	if (pEvent->Type() == EventType::EVENT_OnCollide)
	{
		//check for selectible type and + / - depending on the type
		OnCollideData* pData = pEvent->Data<OnCollideData>();
		if (pData->pGO->m_tag == T_Cursor)
		{
			if (TETRA_INPUT.IsMouseButtonReleased(MOUSEBTN::MOUSE_BTN_LEFT))
			{
				if (m_isLeftSelectable)
				{
					--m_pList->m_currentChoice;
					if (m_pList->m_currentChoice < 0)
						m_pList->m_currentChoice = m_pList->m_Choice.size()-1;

					std::cout << "Current Choice : " << m_pList->m_currentChoice << std::endl;
				}
				else if(m_isRightSelectable)
				{
					m_pList->m_currentChoice++;
					if (m_pList->m_currentChoice >= m_pList->m_Choice.size())
						m_pList->m_currentChoice = 0;

					std::cout << "Current Choice : " << m_pList->m_currentChoice << std::endl;
				}
			}
		}
	}
}

void SelectableButton::LateInitialize()
{
}

void SelectableButton::SetSelectableOwner(GameObject *owner)
{
	m_pOwner = owner;
	if (m_pList == nullptr) {
		if (m_pOwner != nullptr)
			m_pList = m_pOwner->GetComponent<ListSelector>(ComponentType::C_ListSelector);
		else {
			printf("No Game Object owner found. ListSelector component failed to operate.\n");
			return;
		}

		if (m_pList == nullptr) {
			printf("No ListSelector component found. ListSelector component failed to operate.\n");
			assert(m_pList);
			return;
		}
	}
}
