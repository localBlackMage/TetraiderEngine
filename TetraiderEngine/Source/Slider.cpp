#include "Stdafx.h"
#include "Slider.h"

Slider::Slider():Component(ComponentType::C_Slider)
{
	m_minX = 0;
	m_maxX = 100;
	m_isPressed = false;
	m_gotEndpoints = false;
}

Slider::~Slider()
{
}

void Slider::LateInitialize()
{
	if (!m_pTransform)
	{
		if (pGO)
			m_pTransform = pGO->GetComponent<Transform>(ComponentType::C_Transform);
		else {
			printf("No Game Object found. Slider component failed to operate.\n");
			return;
		}

		if (!m_pTransform) {
			printf("No Transform component found. Slider component failed to operate.\n");
			assert(m_pTransform);
			return;
		}
	}
}
void Slider::Update(float)
{
	if (TETRA_INPUT.IsMouseButtonReleased(MOUSEBTN::MOUSE_BTN_LEFT))
	{
		m_isPressed = false;
	}
	if (m_isPressed)
	{
		float x = m_pfollow->GetMousePosition().x;
		if (x < m_pEndpoints->m_start)
			x = m_pEndpoints->m_start;
		else if(x > m_pEndpoints->m_end)
			x = m_pEndpoints->m_end;

		m_pTransform->SetPosition(Vector3D(x, m_pTransform->GetPosition().y, 0));
			
	}
	std::cout << m_pTransform->GetPosition().x<<std::endl;
}

void Slider::Serialize(const json & j)
{
}

void Slider::HandleEvent(Event * pEvent)
{
	if (pEvent->Type() == EVENT_OnCollide)
	{	
		OnCollideData* pData = pEvent->Data<OnCollideData>();
		if (pData->pGO->m_tag==T_Cursor)
		{
			m_pfollow = pData->pGO->GetComponent<FollowCursor>(ComponentType::C_FollowCursor);
			if (TETRA_INPUT.IsMouseButtonPressed(MOUSEBTN::MOUSE_BTN_LEFT))
			{
				m_isPressed = true;
			}
			else if (TETRA_INPUT.IsMouseButtonReleased(MOUSEBTN::MOUSE_BTN_LEFT))
			{
				m_isPressed = false;
			}
		}
		else if (pData->pGO->m_tag == T_SliderBar && m_gotEndpoints==false)
		{
			m_pEndpoints = pData->pGO->GetComponent<Endpoints>(ComponentType::C_Endpoints);
			if (!m_pEndpoints) {
				printf("No Endpoints component found. Endpoints component failed to operate.\n");
				assert(m_pEndpoints);
				return;
			}
			m_gotEndpoints = true;
		}
		
	}
}
