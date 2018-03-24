#pragma once
#include "Component.h"
#include "GameObject.h"
#include "ScriptedAnimation.h"

struct CutSceneInfo {
	CutSceneInfo(GameObject* _pGo, bool isDisablePreviousRendering, bool isParentPrev) : m_pGO(_pGo), m_isDisablePreviousRendering(isDisablePreviousRendering), m_isParentPrevious(isParentPrev) {}
	GameObject* m_pGO;
	bool m_isDisablePreviousRendering;
	bool m_isParentPrevious;
};

class CutScene :public Component
{
private:
	std::vector<CutSceneInfo> m_cutSceneObjects;
	int m_count;
	ScriptedAnimation* mp_Animation;
	bool m_isCutsceneOver;
	void DisablePrevRendering();
	void ParentPrevRendering();
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



