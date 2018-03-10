#include "Stdafx.h"
#include "UIManager.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}

void UIManager::ActivateCanvas(int canvasType)
{
	for (auto can : m_Canvas)
	{
		if (can->m_canvasType == canvasType)
		{
			can->ActivateCanvas();
		}
	}
}

void UIManager::DeactivateCanvas(int canvasType)
{
	for (auto can : m_Canvas)
	{
		if (can->m_canvasType == canvasType)
		{
			can->DeactivateCanvas();
		}
	}
}

void UIManager::RegisterCanvas(Canvas* c)
{
	m_Canvas.push_back(c);
}
