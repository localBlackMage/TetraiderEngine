#include "Stdafx.h"
#include "Canvas.h"


Canvas::Canvas():Component(ComponentType::C_Canvas), m_isAnimateOnActivation(false), m_isDeactivating(false)
{
}

Canvas::~Canvas()
{
	for (auto obj : m_UIelements)
	{
		obj->Destroy();
	}
	TETRA_UI.DeregisterCanvas(this);
}

void Canvas::Update(float)
{
	/*if (!m_isActive)
	{
		DeactivateCanvas();
	}*/
	if (m_isDeactivating) {
		if (!pGO->GetComponent<ScriptedAnimation>(C_ScriptedAnimation)->IsPlaying()) {
			DeactivateCanvas();
			m_isDeactivating = false;
		}
	}
}

void Canvas::Serialize(const json & j)
{
	std::string prefabName;
	m_isActiveOnAwake= ParseBool(j, "isActiveOnAwake");
	m_isAnimateOnActivation = ParseBool(j, "isAnimateOnActivation");
	m_isActive = m_isActiveOnAwake;
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
		if (pObject->m_tag != T_Cursor) {
			pObject->SetParent(this->pGO);
		}
	}

	TETRA_UI.RegisterCanvas(this);

	if (m_isActiveOnAwake)
		ActivateCanvas();
	else
		DeactivateCanvas(true);
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
	}	
}

void Canvas::AddGameObjectToCanvas(GameObject* _pGO) {
	m_UIelements.push_back(_pGO);
}

void Canvas::HandleEvent(Event * pEvent)
{

}

void Canvas::ActivateCanvas()
{
	m_isActive = true;
	for (auto obj : m_UIelements)
	{
		//HACK
		if (obj->m_tag == T_Cursor) {
			obj->m_isCollisionDisabled = false;
			obj->m_isRender = true;
			continue;
		}
		obj->SetActive(true);
	}

	if (m_isAnimateOnActivation) {
		if (pGO->HasComponent(C_ScriptedAnimation))
			pGO->GetComponent<ScriptedAnimation>(C_ScriptedAnimation)->PlayAnimation();
	}
}

void Canvas::DeactivateCanvas(bool isForceDeactivation)
{
	if (m_isAnimateOnActivation && !m_isDeactivating && !isForceDeactivation) {
		DeactivateAfterAnimComplete();
		return;
	}

	m_isActive = false;
	for (auto obj : m_UIelements)
	{
		// HACK
		if (obj->m_tag == T_Cursor) {
			obj->m_isCollisionDisabled = true;
			obj->m_isRender = false;
			continue;
		}
		obj->SetActive(false);
	}
}

void Canvas::DeactivateAfterAnimComplete() {
	if (m_isAnimateOnActivation) {
		if (pGO->HasComponent(C_ScriptedAnimation))
			pGO->GetComponent<ScriptedAnimation>(C_ScriptedAnimation)->PlayAnimation(true);
	}

	m_isDeactivating = true;
}

