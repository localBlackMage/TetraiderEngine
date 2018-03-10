#include "Stdafx.h"
#include "Canvas.h"


Canvas::Canvas():Component(ComponentType::C_Canvas)
{
}


Canvas::~Canvas()
{
}

void Canvas::Update(float)
{
	if (!m_isActive)
	{
		Deactivate();
	}
}

void Canvas::Serialize(const json & j)
{
	m_isActiveOnAwake= ParseBool(j, "isActiveOnAwake");
	m_canvasType= ParseFloat(j, "canvasType");
}

void Canvas::LateInitialize()
{

}

void Canvas::HandleEvent(Event * pEvent)
{
}

void Canvas::ActivateCanvas()
{
	for (auto obj : m_UIelements)
	{
		obj->SetActive(true);
	}
}

void Canvas::DeactivateCanvas()
{
	for (auto obj : m_UIelements)
	{
		obj->Deactivate();
	}
}
