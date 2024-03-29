/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#pragma once

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
	int m_levelToLoad;
	int m_outroLevel;
};

