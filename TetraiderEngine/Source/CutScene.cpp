#include "Stdafx.h"
#include "CutScene.h"


CutScene::CutScene():Component(ComponentType::C_Cutscene), m_isCutsceneOver(false)
{
	m_count = 0;
	TETRA_EVENTS.Subscribe(EVENT_INPUT_PAUSEGAME, this);
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
		if (m_isIntro) {
			TETRA_LEVELS.ActivateRandomGeneration(true);
			TETRA_LEVELS.ChangeLevel(0);
		}
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
		prefabName = ParseString(j["Scenes"][i], "prefab");
		m_cutSceneObjects.push_back(CutSceneInfo(
			TETRA_GAME_OBJECTS.CreateGameObject(ParseString(j["Scenes"][i], "prefab"), true, ParseVector3D(j["Scenes"][i], "position")),
			ParseBool(j["Scenes"][i], "isDisablePreviousRendering"),
			ParseBool(j["Scenes"][i], "isParentPrev")
		));
		m_cutSceneObjects[i].m_pGO->GetComponent<ScriptedAnimation>(C_ScriptedAnimation)->SetInitialPos(ParseVector3D(j["Scenes"][i], "position"));
	}

	m_isIntro = ParseBool(j, "isIntro");
	m_isOutro = ParseBool(j, "isOutro");
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
	else if (pEvent->Type() == EVENT_INPUT_PAUSEGAME) {
		InputButtonData* pData = pEvent->Data<InputButtonData>();
		if (pData->m_isTrigger) {
			TETRA_LEVELS.ActivateRandomGeneration(true);
			TETRA_LEVELS.ChangeLevel(2);
		}
	}
}

void CutScene::LateInitialize()
{
	
}
