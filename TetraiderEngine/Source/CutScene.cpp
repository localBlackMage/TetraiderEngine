#include "Stdafx.h"
#include "CutScene.h"


CutScene::CutScene():Component(ComponentType::C_Cutscene), m_isCutsceneOver(false)
{
	m_count = 0;
}

CutScene::~CutScene()
{
}

void CutScene::Update(float dt)
{
	if (!m_isCutsceneOver)
	{	
		if (!m_cutSceneObjects[m_count].m_pGO->GetComponent<ScriptedAnimation>(ComponentType::C_ScriptedAnimation)->IsPlaying())
		{
			if((int)m_cutSceneObjects.size() - 1 == m_count)
				m_isCutsceneOver = true;
			else {
				++m_count;
				if (m_cutSceneObjects[m_count].m_isDisablePreviousRendering)
					DisablePrevRendering();

				if (m_cutSceneObjects[m_count].m_isParentPrevious)
					ParentPrevRendering();

				m_cutSceneObjects[m_count].m_pGO->GetComponent<ScriptedAnimation>(ComponentType::C_ScriptedAnimation)->PlayAnimation();
			}	
		}
	}
	else
	{

	}
}

void CutScene::DisablePrevRendering() {
	for (int i = m_count - 1; i >= 0; --i) {
		m_cutSceneObjects[i].m_pGO->m_isRender = false;
	}
}

void CutScene::ParentPrevRendering() {
	for (int i = m_count - 1; i >= 0; --i) {
		m_cutSceneObjects[i].m_pGO->SetParent(m_cutSceneObjects[m_count].m_pGO);
	}
}

void CutScene::Serialize(const json & j)
{
	std::string prefabName;
	int numberOfElements = j["Scenes"].size();
	for (int i = 0; i<numberOfElements; ++i)
	{
		m_cutSceneObjects.push_back(CutSceneInfo(
			TETRA_GAME_OBJECTS.CreateGameObject(ParseString(j["Scenes"][i], "prefab"), true, ParseVector3D(j["Scenes"][i], "position")),
			ParseBool(j["Scenes"][i], "isDisablePreviousRendering"),
			ParseBool(j["Scenes"][i], "isParentPrev")
		));
	}
}

void CutScene::HandleEvent(Event * pEvent)
{
	if (pEvent->Type() == EVENT_OnLevelInitialized) {
		if (m_cutSceneObjects.size() == 0) {
			m_isCutsceneOver = true;
			return;
		}

		m_cutSceneObjects[m_count].m_pGO->GetComponent<ScriptedAnimation>(ComponentType::C_ScriptedAnimation)->PlayAnimation();
	}
}

void CutScene::LateInitialize()
{
	
}
