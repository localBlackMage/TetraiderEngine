/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Sujay Shah>
- End Header --------------------------------------------------------*/

#include "Stdafx.h"
#include "Slider.h"

Slider::Slider():Component(ComponentType::C_Slider)
{
	m_minX = 0;
	m_maxX = 100;
	m_isPressed = false;
	m_diff = 0.0f;
	TETRA_EVENTS.Subscribe(EventType::EVENT_OnCanvasActivated, this);
	m_val = 0;
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

	GameObject* o = TETRA_GAME_OBJECTS.CreateGameObject(m_barPrefab, true, m_pTransform->GetPosition());
	TETRA_UI.AddGameObjectToCanvas(CanvasType::CANVAS_SOUNDS, o);
	m_pEndpoints = o->GetComponent<Endpoints>(C_Endpoints);
	m_diff = m_pEndpoints->GetMax() - m_pEndpoints->GetMin();
}

void Slider::Update(float)
{
	pGO->GetComponent<Sprite>(C_Sprite)->SetVOffset(0.0f);
	if (TETRA_INPUT.IsMouseButtonReleased(MOUSEBTN::MOUSE_BTN_LEFT))
	{
		m_isPressed = false;
		if(m_pfollow)
			m_pfollow->m_isSliderBallSeleced = false;
	}
	if (m_isPressed)
	{
		pGO->GetComponent<Sprite>(C_Sprite)->SetVOffset(0.6666f);
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
			
		m_pTransform->SetPosition(Vector3D(x, m_pTransform->GetLocalPosition().y, 0));
			
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
	m_sliderBallType = (SliderBallType)ParseInt(j, "type");
	m_barPrefab = ParseString(j, "barPrefab");
}

void Slider::HandleEvent(Event * pEvent)
{
	if (pEvent->Type() == EVENT_OnCollide) 
	{
		OnCollideData* pData = pEvent->Data<OnCollideData>();
		if (pData->pGO->m_tag == T_Cursor && m_isPressed == false)
		{
			pGO->GetComponent<Sprite>(C_Sprite)->SetVOffset(0.3333f);
			m_pfollow = pData->pGO->GetComponent<FollowCursor>(ComponentType::C_FollowCursor);
			if (m_pfollow->m_isSliderBallSeleced) return;
			if (TETRA_INPUT.IsMouseButtonPressed(MOUSEBTN::MOUSE_BTN_LEFT))
			{
				m_isPressed = true;
				m_pfollow->m_isSliderBallSeleced = true;
			}
		}
	}
	if (pEvent->Type() == EVENT_OnCanvasActivated)
	{
		CanvasData* pCanvasData = pEvent->Data<CanvasData>();
		if (pCanvasData->m_canvasType == (int)CanvasType::CANVAS_SOUNDS)
		{
			if (m_sliderBallType == SliderBallType::SLIDER_MASTER)
			{
				m_val = TETRA_AUDIO.GetMasterVolume();
			}
			else if (m_sliderBallType == SliderBallType::SLIDER_BGM)
			{
				m_val = TETRA_AUDIO.GetSongsVolume();
			}
			else if (m_sliderBallType == SliderBallType::SLIDER_SFX)
			{
				m_val = TETRA_AUDIO.GetSFXsVolume();
			}

			m_val = m_val * m_diff;
			m_val += m_pEndpoints->GetMin();
			m_pTransform->SetPosition(Vector3D(m_val, m_pTransform->GetLocalPosition().y, 0));
		}
	}
}