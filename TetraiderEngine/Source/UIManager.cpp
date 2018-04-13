#include "Stdafx.h"
#include "UIManager.h"

UIManager::UIManager()
{
	TETRA_EVENTS.Subscribe(EVENT_ExitLevel, this);
	TETRA_EVENTS.Subscribe(EVENT_LevelInComplete, this);
	TETRA_EVENTS.Subscribe(EVENT_OnPauseGame, this);
	TETRA_EVENTS.Subscribe(EVENT_OnGameResume, this);
	m_count = 1;
	m_ignoreButtonsForOneFrame = false;
}

UIManager::~UIManager()
{
}

void UIManager::ActivateCanvas(CanvasType c)
{
	if (m_Canvas[(int)c] != nullptr)
		m_Canvas[(int)c]->ActivateCanvas();
	else
		std::cout<<"no canvas to activate\n";
}

void UIManager::DeactivateCanvas(CanvasType c)
{
	if (m_Canvas[(int)c] !=nullptr)
		m_Canvas[(int)c]->DeactivateCanvas();
	else
		std::cout << "no canvas to de-activate\n";
}

void UIManager::RegisterCanvas(Canvas* c)
{
	//std::cout << "Canvas #  "<<(int)c->m_canvasType << std::endl;
	m_Canvas[(int)c->m_canvasType] = c;
}

bool UIManager::IsCanvasAvailable(const CanvasType c) {
		return m_Canvas[(int)c];
}

void UIManager::DeregisterCanvas(Canvas* c)
{
	m_Canvas[(int)c->m_canvasType] = nullptr;
}

void UIManager::DeActivateAllCanvas()
{
	for (int i = 1; i < (int)CanvasType::CANVAS_LOSE; ++i)
	{
		DeactivateCanvas((CanvasType)i);
	}
}

void UIManager::HandleEvent(Event * pEvent)
{
	if (pEvent->Type() == EVENT_OnPauseGame)
	{
		m_Canvas[(int)CanvasType::CANVAS_PAUSE]->ActivateCanvas();
	}
	else if (pEvent->Type() == EVENT_OnGameResume) {
		DeActivateAllCanvas();
	}
	else if (pEvent->Type() == EVENT_LevelInComplete)
	{
		m_Canvas[(int)CanvasType::CANVAS_LOSE]->ActivateCanvas();
	}
}

void UIManager::AddGameObjectToCanvas(CanvasType type, GameObject* _pGO)
{
	m_Canvas[(int)type]->AddGameObjectToCanvas(_pGO);
	_pGO->SetParent(m_Canvas[(int)type]->pGO);
}
