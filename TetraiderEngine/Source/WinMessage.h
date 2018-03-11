#pragma once
//#include "Component.h"
//
//class Text;

class WinMessage :public Component
{
public:
	WinMessage();
	~WinMessage();
	static Component* CreateInstance() { return new WinMessage(); }
	void Update(float dt);
	void Serialize(const json& j);
	void HandleEvent(Event* pEvent);
	void LateInitialize();
private:
	ScriptedAnimation* m_pScriptedAnim;
	WaveMovement* m_pWaveMovement;
	bool m_isMessageOn;
	bool m_isExitingLevel;
	bool m_isRightTransitionSpawned;
	std::string m_rightTransitionPrefab;
	float m_timeToSpawnRightTransition;
	float m_timeToLoadNextLevel;
	float m_timer;
};

