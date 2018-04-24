/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Sujay Shah>
- End Header --------------------------------------------------------*/

#pragma once
#include "Component.h"
#include "GameObject.h"
#include "ScriptedAnimation.h"

struct CutSceneInfo {
	CutSceneInfo(GameObject* _pGo, bool isDisablePreviousRendering, bool isParentPrev, bool isPlaySound, float delaySound, int soundIndex) :
		m_pGO(_pGo), 
		m_isDisablePreviousRendering(isDisablePreviousRendering),
		m_isParentPrevious(isParentPrev),
		m_isPlaySound(isPlaySound),
		m_delaySound(delaySound),
		m_soundIndex(soundIndex)
	{}
	GameObject* m_pGO;
	bool m_isDisablePreviousRendering;
	bool m_isParentPrevious;
	bool m_isPlaySound;
	float m_delaySound;
	int m_soundIndex;
};

class CutScene :public Component
{
private:
	std::vector<CutSceneInfo> m_cutSceneObjects;
	int m_count;
	ScriptedAnimation* mp_Animation;
	bool m_isIntro;
	bool m_isCutsceneOver;
	void DisablePrevRendering();
	void ParentPrevRendering();
	float timeDelay;
	int m_levelToLoad;
	int m_outroLevel;
public:
	CutScene();
	~CutScene();

	static Component* CreateInstance() { return new CutScene(); }
	// Inherited via Component
	 void Update(float dt);
	 void Serialize(const json & j);
	 void HandleEvent(Event* pEvent);
	 void LateInitialize();
};



