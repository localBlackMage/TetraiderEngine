#include "Stdafx.h"
#include "Canvas.h"


Canvas::Canvas():Component(ComponentType::C_Canvas)
{
}

Canvas::~Canvas()
{
	for (auto obj : m_UIelements)
	{
		obj->Destroy();
	}
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
	std::string prefabName;
	m_isActiveOnAwake= ParseBool(j, "isActiveOnAwake");
	m_canvasType = (CanvasType)ParseInt(j, "canvasType");

	int numberOfElements = j["UIElements"].size();
	float x, y, z;
	for (int i = 0; i<numberOfElements; ++i)
	{
		GameObject *pObject;
		prefabName = ParseString(j["UIElements"][i], "prefab");
		x = ParseFloat(j["UIElements"][i]["position"], "x");
		y = ParseFloat(j["UIElements"][i]["position"], "y");
		z = ParseFloat(j["UIElements"][i]["position"], "z");

		pObject = TETRA_GAME_OBJECTS.CreateGameObject(prefabName, true, Vector3D(x, y, z));
		m_UIelements.push_back(pObject);
	}

	TETRA_UI.RegisterCanvas(this);

	if (m_isActiveOnAwake)
		TETRA_UI.ActivateCanvas(m_canvasType);
	else
		TETRA_UI.DeactivateCanvas(m_canvasType);
}

void Canvas::LateInitialize()
{

}

void Canvas::AddGameObjectToCanvas(GameObject* _pGO) {
	m_UIelements.push_back(_pGO);
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
		obj->SetActive(false);
	}
}
