#include "Stdafx.h"
#include "CutScene.h"


CutScene::CutScene():Component(ComponentType::C_Cutscene), m_isCutsceneOver(false)
{
	m_count = 0;
	timeDelay = 0;
	TETRA_EVENTS.Subscribe(EVENT_INPUT_SCREENBYPASS, this);
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
				timeDelay = 0;
				if (m_cutSceneObjects[m_count].m_isDisablePreviousRendering)
					DisablePrevRendering();

				if (m_cutSceneObjects[m_count].m_isParentPrevious)
					ParentPrevRendering();

				m_cutSceneObjects[m_count].m_pGO->GetComponent<ScriptedAnimation>(ComponentType::C_ScriptedAnimation)->PlayAnimation();
			}	
		}
		else if (m_cutSceneObjects[m_count].m_isPlaySound) {
			if (timeDelay <= m_cutSceneObjects[m_count].m_delaySound) {
				timeDelay += dt;
				if (timeDelay > m_cutSceneObjects[m_count].m_delaySound) {
					pGO->GetComponent<Audio>(ComponentType::C_Audio)->Play(m_cutSceneObjects[m_count].m_soundIndex);
				}
			}
		}
	}
	else
	{
		TETRA_AUDIO.StopAllSFXs();
		if (m_isIntro) {
			TETRA_LEVELS.ActivateRandomGeneration(true);
		}
		else {
			TETRA_LEVELS.ActivateRandomGeneration(false);
		}
		TETRA_LEVELS.ChangeLevel(m_levelToLoad);
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
			ParseBool(j["Scenes"][i], "isParentPrev"),
			ParseBool(j["Scenes"][i], "isPlaySound"),
			ParseFloat(j["Scenes"][i], "delaySound"),
			ParseInt(j["Scenes"][i], "soundIndex")
		));
		m_cutSceneObjects[i].m_pGO->GetComponent<ScriptedAnimation>(C_ScriptedAnimation)->SetInitialPos(ParseVector3D(j["Scenes"][i], "position"));
	}

	m_isIntro = ParseBool(j, "isIntro");
	m_levelToLoad = ParseInt(j, "levelToLoad");
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
	else if (pEvent->Type() == EVENT_INPUT_SCREENBYPASS) {
		InputButtonData* pData = pEvent->Data<InputButtonData>();
		if (pData->m_isTrigger) {
			TETRA_AUDIO.StopAllSFXs();
			if (m_isIntro) {
				TETRA_LEVELS.ActivateRandomGeneration(true);
			}
			else {
				TETRA_LEVELS.ActivateRandomGeneration(false);
			}
			TETRA_LEVELS.ChangeLevel(m_levelToLoad);
		}
	}
}

void CutScene::LateInitialize()
{
	
}
