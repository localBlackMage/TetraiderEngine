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
	if (m_canvasType == CanvasType::CANVAS_PAUSE)
	{
		if (!m_pText) {
			if (pGO)
				m_pText = pGO->GetComponent<Text>(ComponentType::C_Text);
			else {
				printf("No Game Object found. Canvas component failed to operate.\n");
				return;
			}

			if (!m_pText) {
				printf("No Text component found. Canvas component failed to operate.\n");
				assert(m_pText);
				return;
			}
		}
		TETRA_EVENTS.Subscribe(EVENT_INPUT_PAUSEGAME, this);
		TETRA_EVENTS.Subscribe(EVENT_ExitLevel, this);
		TETRA_EVENTS.Subscribe(EVENT_LevelInComplete, this);
	}
}

void Canvas::HandleEvent(Event * pEvent)
{

	if (pEvent->Type() == EVENT_INPUT_PAUSEGAME)
	{
		if (m_isLevelOver || m_isCannotPause) return;

		InputButtonData* pData = pEvent->Data<InputButtonData>();
		if (pData->m_isTrigger) {
			m_isActive = !m_isActive;
			
		}
		if (m_isActive)
		{
			this->ActivateCanvas();
			if (m_pText) {
				m_pText->SetText("PAUSED");
				m_pText->SetOffset(Vector3D(-80, 200, 0));
			}
		}	
		else
		{
			this->DeactivateCanvas();
			if (m_pText) {
				m_pText->SetText("");
				//m_pText->SetOffset(Vector3D(-80, 200, 0));
			}
		}	
	}
	
	else if (pEvent->Type() == EVENT_LevelInComplete) {
		m_isLevelOver = true;
		m_isActive = true;

		/*for (auto gameObjects : m_objects) {
			gameObjects->SetActive(m_isActive);
		}

		pGO->m_isRender = m_isActive;*/
		this->ActivateCanvas();

		/*if (m_pText) {
			m_pText->SetText("YOU LOSE");
			m_pText->SetOffset(Vector3D(-120, 200, 0));
		}*/
	}
	else if (pEvent->Type() == EVENT_ExitLevel) {
		m_isCannotPause = true;
	}
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
		if (obj->m_tag == T_Cursor)
			continue;
		obj->SetActive(false);
	}
}
