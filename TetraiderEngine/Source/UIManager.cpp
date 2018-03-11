#include "Stdafx.h"
#include "UIManager.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}

void UIManager::ActivateCanvas(CanvasType c)
{
	if(!m_Canvas[(int)c])
	m_Canvas[(int)c]->ActivateCanvas();
}

void UIManager::DeactivateCanvas(CanvasType c)
{
	if (!m_Canvas[(int)c])
	m_Canvas[(int)c]->DeactivateCanvas();
}

void UIManager::RegisterCanvas(Canvas* c)
{
	m_Canvas[(int)c->m_canvasType] = c;
}

void UIManager::DeregisterCanvas(Canvas* c)
{
	m_Canvas[(int)c->m_canvasType] = nullptr;
}
