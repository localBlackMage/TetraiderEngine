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

void UIManager::DeregisterCanvas(Canvas* c)
{
	m_Canvas[(int)c->m_canvasType] = nullptr;
}

void UIManager::AddGameObjectToCanvas(CanvasType type, GameObject* _pGO) {
	m_Canvas[(int)type]->AddGameObjectToCanvas(_pGO);
}
