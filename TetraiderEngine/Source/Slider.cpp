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
		if (x < m_pEndpoints->GetMin())
		{
			x = m_pEndpoints->GetMin();
			m_currentValue = m_minX;
		}			
		else if (x > m_pEndpoints->GetMax())
		{
			x = m_pEndpoints->GetMax();
			m_currentValue = m_maxX;
		}
			
		m_pTransform->SetPosition(Vector3D(x, m_pTransform->GetPosition().y, 0));
			
		m_currentValue = (x-m_pEndpoints->GetMin()) / m_diff;
		//std::cout <<"Value "<< m_currentValue << std::endl;
		if(m_pEndpoints->GetSliderName()=="BGM_slider")
			TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_ChangeBGMVol, &FloatData(m_currentValue)));
		else if(m_pEndpoints->GetSliderName() == "SFX_slider")
			TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_ChangeSFXVol, &FloatData(m_currentValue)));
		else if(m_pEndpoints->GetSliderName() == "MasterVol_slider")
			TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_ChangeMasterVol, &FloatData(m_currentValue)));
	}
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
			m_diff = m_pEndpoints->GetMax() - m_pEndpoints->GetMin();
			m_gotEndpoints = true;
		}
		
	}
}
